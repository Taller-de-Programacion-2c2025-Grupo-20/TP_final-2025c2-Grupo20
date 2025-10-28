#include "sender.h"

#include "../common/liberror.h"

void Sender::run() {

    while (should_keep_running()) {
        try {
            //ServerState response = client_queue.pop();
            //protocol.sendResponse(response);
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

Sender::Sender(ServerProtocol& protocol, Queue<ServerState>& client_queue):
        protocol(protocol), client_queue(client_queue) {}
