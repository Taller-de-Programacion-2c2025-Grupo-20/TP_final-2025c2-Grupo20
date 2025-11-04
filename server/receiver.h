#ifndef RECEIVER_H
#define RECEIVER_H

#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"

#include "server_protocol.h"

class Receiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<InputCmd>& gameloop_queue;

    uint8_t id;

public:
    void run() override;

    void stop() override;

    Receiver(ServerProtocol& protocol, Queue<InputCmd>& gameloop_queue, uint8_t id);
};

#endif
