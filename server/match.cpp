#include "match.h"

#include <iostream>

Match::Match(uint8_t id, const std::string& name):
        match_id(id),
        name(name),
        running(false),
        clients_queues(),
        gameloop_queue(1024),
        gameloop(gameloop_queue, clients_queues),
        map_id(0),
        keep_cleaning(true) {
    this->map_name = LIBERTY_CITY;
    cleaner_thread = std::thread(&Match::cleaner_loop, this);
}

Match::~Match() {
    keep_cleaning = false;
    if (cleaner_thread.joinable())
        cleaner_thread.join();
    stop();
}

void Match::cleaner_loop() {
    while (keep_cleaning) {
        reap_dead_clients();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}
void Match::start() {
    if (running.exchange(true)) {
        return;
    }
    gameloop.loadMapData(this->map_name);
    std::lock_guard<std::mutex> lock(clients_mtx);

    for (auto& client: clients) {
        client->send_start_game();
    }

    for (auto& client: clients) {
        uint8_t p_id = client->get_id();
        CarType type = CarType::VERDE;
        if (player_cars.find(p_id) != player_cars.end()) {
            type = player_cars[p_id];
        }
        gameloop.addCar(p_id, type);
    }
    gameloop.start();
}

void Match::stop() {
    bool was_running = running.exchange(false);
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (auto& client: clients) {
        client->stop();
    }
    if (was_running) {
        gameloop.stop();
    }
    if (was_running) {
        gameloop.join();
    }
    for (auto& client: clients) {
        client->join();
    }
    clients.clear();
}

bool Match::is_full() const { return clients.size() >= MAX_PLAYERS; }

bool Match::is_running() const { return running; }

bool Match::add_player(std::unique_ptr<ClientHandler> new_player) {
    if (is_full() || is_running()) {
        return false;
    }

    std::lock_guard<std::mutex> lock(clients_mtx);
    uint8_t player_id = new_player->get_id();

    Queue<GameStateDTO>& sender_queue = clients_queues.addQueue(player_id);

    new_player->connect_to_match(gameloop_queue, sender_queue);
    clients.push_back(std::move(new_player));

    return true;
}

void Match::reap_dead_clients() {

    std::lock_guard<std::mutex> lock(clients_mtx);
    bool player_removed = false;
    clients.remove_if([this, &player_removed](const std::unique_ptr<ClientHandler>& client) {
        if (client->is_alive())
            return false;

        player_removed = true;
        uint8_t dead_id = client->get_id();
        std::cout << "Limpiando jugador desconectado ID: " << (int)dead_id << std::endl;

        try {
            clients_queues.markQueueForDeletion(dead_id);
        } catch (const std::exception& e) {
            std::cerr << "Warning limpiando cola de cliente " << (int)dead_id << ": " << e.what()
                      << std::endl;
        }

        try {
            client->stop();
        } catch (...) {}

        try {
            client->join();
        } catch (...) {}

        if (player_cars.count(dead_id)) {
            player_cars.erase(dead_id);
        }

        return true;
    });
}

bool Match::has_player(uint8_t player_id) {
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (const auto& client: clients) {
        if (client->get_id() == player_id) {
            return true;
        }
    }
    return false;
}

void Match::broadcast_waiting_room_state() {
    if (running)
        return;
    LobbyStateDTO state_to_send;

    std::lock_guard<std::mutex> lock(clients_mtx);

    if (clients.empty())
        return;

    state_to_send.host_id = clients.front()->get_id();
    state_to_send.map_id = this->map_id;

    for (const auto& client: clients) {
        LobbyPlayerInfo player_info;
        player_info.player_id = client->get_id();
        player_info.name = client->get_username();
        player_info.car_id = client->get_car_id();
        state_to_send.players.push_back(player_info);
    }

    for (auto& client: clients) {
        client->send_lobby_update(state_to_send);
    }
}

uint8_t Match::get_id() const { return match_id; }

std::string Match::get_name() const { return name; }

size_t Match::get_player_count() {
    std::lock_guard<std::mutex> lock(clients_mtx);
    return clients.size();
}

uint8_t Match::get_host_id() {
    std::lock_guard<std::mutex> lock(clients_mtx);
    if (clients.empty()) {
        return 0;
    }
    return clients.front()->get_id();
}

void Match::set_map_id(uint8_t id) {
    switch (id) {
        case 0:
            map_name = LIBERTY_CITY;
            break;
        case 1:
            map_name = VICE_CITY;
            break;
        case 2:
            map_name = SAN_ANDREAS;
            break;
        default:
            map_name = LIBERTY_CITY;
            break;
    }
    this->map_id = id;
}

void Match::set_player_car(uint8_t player_id, uint8_t car_id) {
    std::lock_guard<std::mutex> lock(clients_mtx);
    player_cars[player_id] = static_cast<CarType>(car_id);

    for (auto& client: clients) {
        if (client->get_id() == player_id) {
            client->set_car_id(car_id);
            break;
        }
    }
}
