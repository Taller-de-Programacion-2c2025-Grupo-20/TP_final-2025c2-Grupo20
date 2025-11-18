#include "client_handler.h"
#include <utility>

ClientHandler::ClientHandler(Socket&& skt, uint8_t id):
        id(id),
        protocol(std::move(skt)),
        is_alive_flag(true),
        receiver(protocol, id),
        sender(protocol)
{}

ClientHandler::~ClientHandler() {}

void ClientHandler::start_in_lobby(Queue<LobbyCommand>& lobby_queue) {
    receiver.set_lobby_queue(lobby_queue);
    receiver.start();
    sender.start();
}

void ClientHandler::connect_to_match(Queue<InputCmd>& game_queue, 
                                     Queue<GameStateDTO>& sender_queue) {
    receiver.set_game_queue(game_queue);
    sender.set_state_queue(sender_queue);
}

void ClientHandler::stop() {
    is_alive_flag = false;
    protocol.close();
    receiver.stop();
    sender.stop();
}

void ClientHandler::join() {
    receiver.join();
    sender.join();
}

bool ClientHandler::is_alive() const {
    return receiver.is_alive() || sender.is_alive();
}

uint8_t ClientHandler::get_id() const { return id; }

void ClientHandler::set_username(const std::string& name) {
    this->username = name;
}

std::string ClientHandler::get_username() const {
    return this->username;
}

void ClientHandler::send_login_ok(uint8_t player_id) {
    try {
        protocol.send_login_ok(player_id);
    } catch (const std::exception& e) {
        is_alive_flag = false;
    }
}

void ClientHandler::send_lobby_update(const LobbyStateDTO& state) {
    try {
        protocol.send_lobby_state(state);
    } catch (const std::exception& e) {
        is_alive_flag = false; 
    }
}

void ClientHandler::send_match_list(const MatchListDTO& list) {
    protocol.send_match_list(list);
}

void ClientHandler::send_start_game() {
    protocol.send_start_game();
}
