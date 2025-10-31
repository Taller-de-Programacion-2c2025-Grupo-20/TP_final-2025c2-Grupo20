#include "client_sender.h"

#include "../common/liberror.h"

void ClientSender::run() {

    while (should_keep_running()) {
        try {
            InputCmd response = client_queue.pop();
            protocol.sendInput(response);
        } catch (const ClosedQueue&) {
            break;
        } catch (const LibError& e) {
            break;
        }
    }
}

void ClientSender::stop() {
    Thread::stop();
    client_queue.close();
}

ClientSender::ClientSender(ClientProtocol& protocol, Queue<InputCmd>& client_queue):
        protocol(protocol), client_queue(client_queue) {}
