#include "match.h"
#include <iostream>

Match::Match(uint8_t id, const std::string& name) :
    match_id(id),
    name(name),
    running(false),
    world(b2Vec2(0,0), true), 
    clients_queues(),
    gameloop_queue(1024), 
    gameloop(gameloop_queue, clients_queues, world) 
{}

Match::~Match() {
    stop();
}

void Match::start() {
    if (running.exchange(true)) {
        return;
    }
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (auto& client : clients) {
        gameloop.add_player(client->get_id());
    }
    gameloop.run(); 
}

void Match::stop() {
    if (!running.exchange(false)) {
        return;
    }
    gameloop.stop();
    gameloop.join();

    std::lock_guard<std::mutex> lock(clients_mtx);
    for (auto& client : clients) {
        client->stop();
        client->join();
    }
    clients.clear();
}

bool Match::is_full() const {
    return clients.size() >= MAX_PLAYERS;
}

bool Match::is_running() const {
    return running;
}

bool Match::add_player(std::unique_ptr<ClientHandler> new_player) {
    if (is_full() || is_running()) {
        return false;
    }

    std::lock_guard<std::mutex> lock(clients_mtx);
    uint8_t player_id = new_player->get_id();

    Queue<GameStateDTO>& sender_queue = clients_queues.addQueue(player_id);

    new_player->connect_to_match(gameloop_queue, sender_queue);
    //gameloop.add_player(player_id);
    clients.push_back(std::move(new_player));

    broadcast_waiting_room_state();
    
    return true;
}

void Match::reap_dead_clients() {
    if (!is_running()) return;
    
    std::lock_guard<std::mutex> lock(clients_mtx);
    bool player_removed = false;
    clients.remove_if([this, &player_removed](const std::unique_ptr<ClientHandler>& client) {
        if (client->is_alive()) {
            return false;
        }
        player_removed = true;
        std::cout << "Limpiando jugador " << (int)client->get_id() << std::endl;
        try {
            clients_queues.markQueueForDeletion(client->get_id());
            client->stop();
            client->join();
        } catch (const std::exception& e) {
            std::cerr << "Error (ignorable) al limpiar cliente " 
                      << (int)client->get_id() << ": " << e.what() << std::endl;
        }

        if (player_removed && !is_running()) {
            broadcast_waiting_room_state(); // <--- ¡¡AÑADIR ESTO!!
        }
        return true; 
    });
}  

bool Match::has_player(uint8_t player_id) {
    std::lock_guard<std::mutex> lock(clients_mtx);
    for (const auto& client : clients) {
        if (client->get_id() == player_id) {
            return true;
        }
    }
    return false;
}

void Match::broadcast_waiting_room_state() {
    if (running) return;
    LobbyStateDTO state_to_send;

    std::lock_guard<std::mutex> lock(clients_mtx);

    if (clients.empty()) return;

    state_to_send.host_id = clients.front()->get_id(); // El host es el primer jugador
    state_to_send.map_id = 0; // O el ID del mapa que estés usando

    // Llena la lista de jugadores
    for (const auto& client : clients) {
        LobbyPlayerInfo player_info;
        player_info.player_id = client->get_id();
        player_info.name = client->get_username(); 
        player_info.is_ready = false; // (O la lógica de "listo" que tengas)

        state_to_send.players.push_back(player_info);
    }

    // Envía el estado a TODOS los clientes en este match
    for (auto& client : clients) {
        client->send_lobby_update(state_to_send); // Asumo que ClientHandler tiene esta función
    }
}

uint8_t Match::get_id() const {
    return match_id;
}

std::string Match::get_name() const {
    return name;
}

size_t Match::get_player_count() {
    std::lock_guard<std::mutex> lock(clients_mtx);
    return clients.size();
}
