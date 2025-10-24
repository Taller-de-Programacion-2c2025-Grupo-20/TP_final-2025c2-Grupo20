#ifndef RECEIVER_H
#define RECEIVER_H

#include "../common_src/queue.h"
#include "../common_src/thread.h"

#include "server_protocol.h"

class Receiver: public Thread {
private:
    ServerProtocol& protocol;
    Queue<Instruction>& gameloop_queue;

    int id;

public:
    void run() override;

    void stop() override;

    Receiver(ServerProtocol& protocol, Queue<Instruction>& gameloop_queue, int id);
};

#endif
