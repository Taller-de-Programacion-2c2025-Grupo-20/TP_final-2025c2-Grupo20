#include "sender.h"
#include "../common/liberror.h"
#include <chrono>

Sender::Sender(ServerProtocol& protocol):
    protocol(protocol),
    client_queue(nullptr)
{}

void Sender::set_state_queue(Queue<GameStateDTO>& new_queue) {
    std::lock_guard<std::mutex> lock(mtx);
    this->client_queue = &new_queue;
}

void Sender::run() {
    try {
        while (should_keep_running()) {
            Queue<GameStateDTO>* current_game_queue = nullptr;
            
            {
                std::lock_guard<std::mutex> lock(mtx);
                current_game_queue = this->client_queue;
            }

            if (current_game_queue) {
                GameStateDTO state_to_send = current_game_queue->pop(); 
                protocol.send_game_state(state_to_send);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }
    } catch (const ClosedQueue&) {
        // La cola se cerró (la partida terminó)
    } catch (const LibError& e) {
        // El socket se rompió.
    }
}

void Sender::stop() {
    Thread::stop();
    std::lock_guard<std::mutex> lock(mtx);
    if (client_queue) {
        client_queue->close();
    }
}