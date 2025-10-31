#ifndef SENDER_H
#define SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/serverState.h"

#include "server_protocol.h"

class Sender: public Thread {
private:
    ServerProtocol& protocol;
    Queue<GameStateDTO>& client_queue;

public:
    void run() override;

    void stop() override;

    Sender(ServerProtocol& protocol, Queue<GameStateDTO>& client_queue);
};

#endif
