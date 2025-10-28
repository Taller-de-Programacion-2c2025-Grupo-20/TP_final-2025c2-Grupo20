#include "receiver.h"

#include "../common/liberror.h"

void Receiver::run() {
    while (should_keep_running()) {
        try {
            uint8_t command = protocol.receiveCommand();
            if (command == 0x0) {
                break;
            }
            //gameloop_queue.push(Instruction(command, id));
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
