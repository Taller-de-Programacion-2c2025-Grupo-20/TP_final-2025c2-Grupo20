#include "sender.h"
#include "../common/liberror.h"
#include "../common/gameState.h"

void Sender::run() {
    while (should_keep_running()) {
        try {
            GameStateDTO state_to_send = client_queue.pop(); 
            protocol.send_game_state(state_to_send);
        } catch (const ClosedQueue&) {
            break;
        } catch (const LibError& e) {
            break;
        }
    }
}

void Sender::stop() {
    Thread::stop();
    client_queue.close();
}

Sender::Sender(ServerProtocol& protocol, Queue<GameStateDTO>& client_queue):
        protocol(protocol), client_queue(client_queue) {}
