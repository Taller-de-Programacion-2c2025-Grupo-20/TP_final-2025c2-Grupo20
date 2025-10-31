#include "receiver.h"
#include "../common/liberror.h"
#include "../common/constants.h"

void Receiver::run() {
    while (should_keep_running()) {
        try {
            uint8_t command_code = protocol.receiveCommand();
            if (command_code == 0x0) {
                break;
            }

            switch (command_code) {
                
                case CMD_LOGIN: {
                    std::string username = protocol.receive_login_attempt();
                    // (Aquí deberías encolar un "LoginCommand"
                    // para que el Server/Lobby lo procese)
                    break;
                }
                
                case CMD_ENVIAR_INPUT: {
                    
                    InputCmd client_command = protocol.receive_input_command();
                    std::cout << static_cast<int>(client_command.action) << "\n";
                    gameloop_queue.push(client_command);
                    break;
                }

                default:
                    // Error, comando desconocido
                    break;
            }
        
        } catch (const ClosedQueue&) {
            break;
        } catch (const LibError&) {
            break;
        }
    }
}

void Receiver::stop() {
    if (!is_alive()) {
        return;
    }
    Thread::stop();
    protocol.close();
}

Receiver::Receiver(ServerProtocol& protocol, Queue<InputCmd>& gameloop_queue, int id):
        protocol(protocol), gameloop_queue(gameloop_queue), id(id) {}