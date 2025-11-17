#include "receiver.h"
#include "../common/liberror.h"
#include "../common/constants.h"
#include <iostream>

Receiver::Receiver(ServerProtocol& protocol, uint8_t id):
    protocol(protocol),
    id(id),
    gameloop_queue(nullptr),
    lobby_queue(nullptr) 
{}

void Receiver::set_lobby_queue(Queue<LobbyCommand>& new_queue) {
    std::lock_guard<std::mutex> lock(mtx);
    this->lobby_queue = &new_queue;
}

void Receiver::set_game_queue(Queue<InputCmd>& new_queue) {
    std::lock_guard<std::mutex> lock(mtx);
    this->gameloop_queue = &new_queue;
}

void Receiver::run() {
    try {
        while (should_keep_running()) {
            uint8_t command_code = protocol.receiveCommand();
            if (command_code == 0x0) break; 

            std::cout << "SERVER DEBUG: Receiver (ID: " << (int)id 
                      << ") recibió el comando: 0x" << std::hex << (int)command_code 
                      << std::dec << std::endl;

            std::lock_guard<std::mutex> lock(mtx);

            switch (command_code) {
                
                case CMD_LOGIN: {
                    if (!lobby_queue) continue;
                    std::string username = protocol.receive_login_attempt();
                    lobby_queue->push(LobbyCommand(LobbyCommandType::LOGIN_ATTEMPT, id, username));
                    break;
                }
                case CMD_CREATE_MATCH: {
                    std::cout << "SERVER DEBUG: Comando es CMD_CREATE_MATCH. Procesando..." << std::endl;
                    if (!lobby_queue) continue;
                    std::string match_name = protocol.receive_create_match_payload();
                    lobby_queue->push(LobbyCommand(LobbyCommandType::CREATE_MATCH, id, match_name));
                    break;
                }
                case CMD_JOIN_MATCH: {
                    if (!lobby_queue) continue;
                    uint8_t match_id = protocol.receiveUint8_t();
                    lobby_queue->push(LobbyCommand(LobbyCommandType::JOIN_MATCH, id, match_id));
                    break;
                }
                case CMD_START_GAME: {
                    if (!lobby_queue) continue;
                    lobby_queue->push(LobbyCommand(LobbyCommandType::START_GAME, id));
                    break;
                }
                case CMD_ENVIAR_INPUT: {
                    if (!gameloop_queue) continue;
                    InputCmd cmd = protocol.receive_input_command();
                    cmd.player_id = this->id;
                    gameloop_queue->push(cmd);
                    break;
                }

                default:
                    std::cerr << "Comando desconocido recibido: " << (int)command_code << std::endl;
                    break;
            }
        
        }
    } catch (const LibError&) {
        // Socket cerrado (probablemente por stop())
    }
}

void Receiver::stop() {
    Thread::stop();
    protocol.close();
}