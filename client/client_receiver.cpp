#include "client_receiver.h"
#include "../common/constants.h"
#include <stdexcept>
#include <iostream>
#include <QDebug>

ClientReceiver::ClientReceiver(ClientProtocol& protocol, QObject* parent) :
    QObject(parent),
    protocol(protocol),
    is_authenticated(false),
    my_player_id(0),
    lobby_state_queue(1),
    game_state_queue(1)
{}

void ClientReceiver::run() {
    try {
        while (should_keep_running()) {
            uint8_t cmd_code;
            if (!protocol.receive_command_code(cmd_code)) {
                break;
            }
            std::cout << "Received command code: " << static_cast<int>(cmd_code) << std::endl;
            switch (cmd_code) {
                case LOGIN_SUCCESS: {
                    uint8_t my_id = protocol.receive_login_response_payload();
                    this->my_player_id = my_id;
                    this->is_authenticated = true;
                    emit loginSuccess(my_id);
                    break;
                }
                
                case RSP_LOBBY_STATE: {
                    LobbyStateDTO new_state = protocol.receive_lobby_state_payload();
                    LobbyStateDTO dummy;
                    lobby_state_queue.try_pop(dummy);
                    lobby_state_queue.push(std::move(new_state));
                    break;
                }

                case EVT_GAME_STATE: {
                    GameStateDTO new_state = protocol.receive_game_state_payload();
                    GameStateDTO dummy;
                    game_state_queue.try_pop(dummy);
                    game_state_queue.push(std::move(new_state));
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        if (should_keep_running()) {
            emit loginFailed();
        }
    }
}

GameStateDTO ClientReceiver::pollGameState() {
    GameStateDTO new_state;
    if (game_state_queue.try_pop(new_state)) {
        std::lock_guard<std::mutex> lock(mtx);
        last_game_state = new_state;
        return new_state;
    } else {
        std::lock_guard<std::mutex> lock(mtx);
        return last_game_state;
    }
}

LobbyStateDTO ClientReceiver::pollLobbyState() {
    LobbyStateDTO new_state;
    if (lobby_state_queue.try_pop(new_state)) {
        std::lock_guard<std::mutex> lock(mtx);
        last_lobby_state = new_state;
        return new_state;
    } else {
        std::lock_guard<std::mutex> lock(mtx);
        return last_lobby_state;
    }
}

bool ClientReceiver::is_logged_in() const { return is_authenticated; }
uint8_t ClientReceiver::get_my_id() const { return my_player_id; }

void ClientReceiver::stop() {
    Thread::stop();
    protocol.close();
}