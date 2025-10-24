#ifndef SENDER_H
#define SENDER_H

#include "../common_src/queue.h"
#include "../common_src/thread.h"

#include "server_protocol.h"

class Sender: public Thread {
private:
    ServerProtocol& protocol;
    Queue<ResponseDTO>& client_queue;

public:
    void run() override;

    void stop() override;

    Sender(ServerProtocol& protocol, Queue<ResponseDTO>& client_queue);
};

#endif
