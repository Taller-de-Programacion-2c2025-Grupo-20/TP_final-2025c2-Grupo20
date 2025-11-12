#include "server.h"
#include <iostream>
#include <utility> 
#include <chrono>
#include <sys/socket.h>
#include "../common/constants.h"


Server::Server(const char* port) :
    listener(port), 
    acceptor(listener, new_clients_queue), 
    is_running(true),
    new_clients_queue(128),
    lobby_queue(128),      
    next_match_id(1)
{
}

Server::~Server() {
    stop();
}

/**
 * @brief Bucle principal del Lobby.
 * Acepta clientes, procesa comandos de lobby y limpia partidas.
 */
int Server::run() {
    acceptor.start(); 
    std::cout << "Servidor (Lobby) corriendo. Presione 'q' para salir." << std::endl;
    input_listener_thread = std::thread([this]() {
        char c;
        while (std::cin.get(c)) { 
            if (c == 'q') {
                this->is_running = false; 
                break;
            }
        }
    });

    while (is_running) {
        try {
            process_new_clients();
            process_lobby_commands();
            reap_dead_lobby_clients();
            cleanup_finished_matches();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } catch (const std::exception& e) {
            std::cerr << "Error en el bucle del Lobby: " << e.what() << std::endl;
        }
    }
    std::cout << "Señal de 'q' recibida, iniciando apagado..." << std::endl;    
    stop(); 
    if (input_listener_thread.joinable()) {
        input_listener_thread.join();
    }
    return 0;
}

void Server::stop() {
    if (!is_running.exchange(false)) return;

    std::cout << "Deteniendo el servidor..." << std::endl;
    try {
        listener.shutdown(SHUT_RDWR);
        listener.close();
    } catch (const std::exception& e) {
        // ...
    }
    
    acceptor.stop();
    acceptor.join();

    lobby_queue.close();
    new_clients_queue.close();

    std::lock_guard<std::mutex> lock(mtx);
    for (auto& pair : active_matches) {
        pair.second->stop();
    }
    active_matches.clear();
    
    for (auto& client : clients_in_lobby) {
        client->stop();
        client->join();
    }
    clients_in_lobby.clear();
}

/**
 * @brief Mueve clientes de la cola "nuevos" a la lista "en el lobby".
 */
void Server::process_new_clients() {
    std::unique_ptr<ClientHandler> new_client;
    while (new_clients_queue.try_pop(new_client)) {
        std::cout << "Nuevo cliente (ID: " << (int)new_client->get_id() << ") conectado al lobby." << std::endl;
        
        new_client->start_in_lobby(lobby_queue); 
        
        std::lock_guard<std::mutex> lock(mtx);
        clients_in_lobby.push_back(std::move(new_client));
    }
}

/**
 * @brief Procesa los comandos de los clientes (ej: "unirse", "crear").
 */
void Server::process_lobby_commands() {
    LobbyCommand cmd;
    while (lobby_queue.try_pop(cmd)) {
        std::lock_guard<std::mutex> lock(mtx); 
        bool state_changed = false;
        
        switch (cmd.type) {
            case LobbyCommandType::LOGIN_ATTEMPT:
                handle_login(cmd);
                state_changed = true;
                break;
            case LobbyCommandType::CREATE_MATCH:
                handle_create_match(cmd);
                break;
            case LobbyCommandType::JOIN_MATCH:
                handle_join_match(cmd);
                break;
            case LobbyCommandType::TOGGLE_READY:
                handle_toggle_ready(cmd);
                state_changed = true;
                break;
        }
        if (state_changed) {
            broadcast_lobby_state();
        }
    }
}

void Server::handle_login(const LobbyCommand& cmd) {
    for (const auto& client : clients_in_lobby) {
        if (client->get_username() == cmd.username && !cmd.username.empty()) {
            std::cerr << "Login fallido: nombre " << cmd.username << " ya en uso." << std::endl;
            return;
        }
    }

    for (auto& client : clients_in_lobby) {
        if (client->get_id() == cmd.client_id) {
            
            client->set_username(cmd.username);
            client->send_login_ok(cmd.client_id); 
            
            LobbyPlayerInfo new_player;
            new_player.player_id = cmd.client_id;
            new_player.name = cmd.username;
            new_player.is_ready = false;
            
            current_lobby_state.players.push_back(new_player);
            if (current_lobby_state.players.size() == 1) {
                current_lobby_state.host_id = cmd.client_id;
            }
            
            broadcast_lobby_state(); 

            return;
        }
    }
}

void Server::broadcast_lobby_state() {
    for (auto& client : clients_in_lobby) {
        client->send_lobby_update(current_lobby_state);
    }
}

void Server::handle_create_match(const LobbyCommand& cmd) {
    std::unique_ptr<ClientHandler> client;
    clients_in_lobby.remove_if([&client, &cmd](auto& c) {
        if (c->get_id() == cmd.client_id) {
            client = std::move(c); 
            return true;
        }
        return false;
    });

    if (!client) return; 

    uint8_t new_id = next_match_id++;
    std::string match_name = "Partida " + std::to_string(new_id);
    auto new_match = std::make_unique<Match>(new_id, match_name);
    new_match->add_player(std::move(client));
    new_match->run(); 
    
    active_matches[new_id] = std::move(new_match);
    std::cout << "Partida " << (int)new_id << " creada por cliente " << (int)cmd.client_id << std::endl;
}

void Server::handle_join_match(const LobbyCommand& cmd) {
    auto it = active_matches.find(cmd.match_id);
    if (it == active_matches.end() || it->second->is_full() || it->second->is_running()) {
        std::cerr << "Error: Partida " << (int)cmd.match_id << " no existe o está llena/en curso." << std::endl;
        return;
    }

    std::unique_ptr<ClientHandler> client;
    clients_in_lobby.remove_if([&client, &cmd](auto& c) {
        if (c->get_id() == cmd.client_id) {
            client = std::move(c); 
            return true;
        }
        return false;
    });

    if (client) {
        if (!it->second->add_player(std::move(client))) {
            // No se pudo unir (ej: se llenó justo), lo devolvemos al lobby
            clients_in_lobby.push_back(std::move(client));
            // (Enviar error al cliente)
        }
    }
}

void Server::handle_toggle_ready(const LobbyCommand& cmd) {
    for (auto& player : current_lobby_state.players) {
        if (player.player_id == cmd.client_id) {
            player.is_ready = !player.is_ready;
            break;
        }
    }
}

void Server::reap_dead_lobby_clients() {
    std::lock_guard<std::mutex> lock(mtx);
    
    clients_in_lobby.remove_if([](const std::unique_ptr<ClientHandler>& client) {
        if (client->is_alive()) {
            return false;
        }

        std::cout << "Limpiando cliente " << (int)client->get_id() << " del lobby." << std::endl;
        try {
            client->stop(); 
            client->join(); 
        } catch (const std::exception& e) {
            std::cerr << "Error (ignorable) al limpiar cliente " 
                      << (int)client->get_id() << ": " << e.what() << std::endl;
        }
        return true; 
    });
}

void Server::cleanup_finished_matches() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto it = active_matches.begin(); it != active_matches.end(); ) {
        if (!it->second->is_running()) {
            std::cout << "Limpiando partida terminada " << (int)it->first << std::endl;
            it = active_matches.erase(it);
        } else {
            ++it;
        }
    }
}