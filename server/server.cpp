#include "server.h"
#include <iostream>
#include <utility> 
#include <chrono>
#include <sys/socket.h>
#include "../common/constants.h"
#include "../common/match_list.h"


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
        
        Match* match_creado = nullptr;
        {
            std::lock_guard<std::mutex> lock(mtx); 
            std::cout << "ENTRE A PROCESAR COMANDO: " << (int)cmd.type << std::endl;
            switch (cmd.type) {
            case LobbyCommandType::LOGIN_ATTEMPT:
                handle_login(cmd);
                break;
            
            case LobbyCommandType::CREATE_MATCH:
                handle_create_match(cmd);
                break;
            
            case LobbyCommandType::JOIN_MATCH:
                handle_join_match(cmd);
                break;
            
            case LobbyCommandType::START_GAME:
                handle_start_game(cmd);
                break;

            case LobbyCommandType::REFRESH_MATCH_LIST: 
                send_match_list(cmd.client_id);
                break;
            }
        }
        if (match_creado) {
            match_creado->broadcast_waiting_room_state();
        }
    }
}

void Server::handle_login(const LobbyCommand& cmd) {
    for (const auto& client : clients_in_lobby) {
        if (client->get_username() == cmd.text_payload && !cmd.text_payload.empty()) {
            std::cerr << "Login fallido: nombre " << cmd.text_payload << " ya en uso." << std::endl;
            return;
        }
    }

    for (auto& client : clients_in_lobby) {
        if (client->get_id() == cmd.client_id) {
            
            client->set_username(cmd.text_payload);
            client->send_login_ok(cmd.client_id); 
            
            std::cout << "Jugador " << (int)cmd.client_id << " se logueó como " << cmd.text_payload << std::endl;
            
            return;
        }
    }
}
/*
void Server::broadcast_lobby_state() {
    LobbyStateDTO state_to_send;
    
    if (!clients_in_lobby.empty()) {
        state_to_send.host_id = clients_in_lobby.front()->get_id();
    } else {
        state_to_send.host_id = 0;
    }
    
    state_to_send.map_id = 0;

    for (const auto& client : clients_in_lobby) {
        LobbyPlayerInfo player_info;
        player_info.player_id = client->get_id();
        player_info.name = client->get_username(); 
        player_info.is_ready = false;
        
        state_to_send.players.push_back(player_info);
    }

    for (auto& client : clients_in_lobby) {
        client->send_lobby_update(state_to_send);
    }
} */


void Server::broadcast_match_list() {
    MatchListDTO list_dto;

    for (const auto& pair : active_matches) {
        const auto& match = pair.second;
        if (!match->is_running() && !match->is_full()) {
            MatchInfo match_info;
            match_info.match_id = match->get_id();
            match_info.name = match->get_name();
            match_info.player_count = match->get_player_count(); 
            list_dto.matches.push_back(match_info);
        }
    }

    for (auto& client : clients_in_lobby) {
        client->send_match_list(list_dto);
    }
}

void Server::send_match_list(uint8_t client_id) {
    ClientHandler* target_client = nullptr;
    for (auto& client : clients_in_lobby) {
        if (client->get_id() == client_id) {
            target_client = client.get();
            break;
        }
    }

    if (!target_client) return;

    MatchListDTO list_dto;
    for (const auto& pair : active_matches) {
        const auto& match = pair.second;
        if (!match->is_running() && !match->is_full()) {
            MatchInfo match_info;
            match_info.match_id = match->get_id();
            match_info.name = match->get_name();
            match_info.player_count = match->get_player_count(); 
            list_dto.matches.push_back(match_info);
        }
    }

    try {
        target_client->send_match_list(list_dto);
    } catch (...) {}
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
    std::string match_name = cmd.text_payload;
    if (match_name.empty()) {
        match_name = "Partida " + std::to_string(new_id);
    }
    
    auto new_match = std::make_unique<Match>(new_id, match_name);
    new_match->add_player(std::move(client));
    
    active_matches[new_id] = std::move(new_match);
    
    std::cout << "Partida " << (int)new_id << " creada. Total: " << active_matches.size() << std::endl;
    
    active_matches[new_id]->broadcast_waiting_room_state();
}

void Server::handle_join_match(const LobbyCommand& cmd) {
    auto it = active_matches.find(cmd.id_payload);
    if (it == active_matches.end() || it->second->is_full() || it->second->is_running()) {
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
        it->second->add_player(std::move(client));
        it->second->broadcast_waiting_room_state();
    }
}



void Server::handle_start_game(const LobbyCommand& cmd) {
    for (auto& pair : active_matches) {
        if (pair.second->has_player(cmd.client_id)) {
            
            // (Aquí iría la lógica de verificar si es el host)
            
            std::cout << "Iniciando partida " << (int)pair.first << "..." << std::endl;
            pair.second->start();
            return;
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
    bool match_removed = false;
    for (auto it = active_matches.begin(); it != active_matches.end(); ) {
        if (!it->second->is_running() && it->second->get_player_count() == 0) {
            std::cout << "Limpiando partida terminada " << (int)it->first << std::endl;
            match_removed = true;
            it = active_matches.erase(it);
        } else {
            ++it;
        }
    }
    if (match_removed) {
        broadcast_match_list();
    }
}