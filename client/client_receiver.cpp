#include "client_receiver.h"

#include <stdexcept>

#include "../common/constants.h"
#include "../common/liberror.h"
#include "client_protocol.h"

ClientReceiver::ClientReceiver(ClientProtocol& protocol, Queue<GameStateDTO>& state_queue) :
    protocol(protocol),
    state_queue(state_queue){}

void ClientReceiver::run() {

    while (should_keep_running()) {
        
        try {
            GameStateDTO current_state = protocol.receive_game_state();
            state_queue.push(current_state);
        } catch (const ClosedQueue&) {
            break;
        } catch (const LibError&) {
            break;
        }
    }
}

void ClientReceiver::stop() {
    Thread::stop();
    protocol.close();
}

