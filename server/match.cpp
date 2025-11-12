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

void Match::run() {
    if (running.exchange(true)) {
        return;
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
    gameloop.add_player(player_id);
    clients.push_back(std::move(new_player));
    
    return true;
}

void Match::reap_dead_clients() {
    if (!is_running()) return;
    
    std::lock_guard<std::mutex> lock(clients_mtx);
    
    clients.remove_if([this](const std::unique_ptr<ClientHandler>& client) {
        if (client->is_alive()) {
            return false;
        }
        std::cout << "Limpiando jugador " << (int)client->get_id() << std::endl;
        try {
            clients_queues.markQueueForDeletion(client->get_id());
            client->stop();
            client->join();
        } catch (const std::exception& e) {
            std::cerr << "Error (ignorable) al limpiar cliente " 
                      << (int)client->get_id() << ": " << e.what() << std::endl;
        }
        return true; 
    });
}
