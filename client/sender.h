#ifndef SENDER_H
#define SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"

#include "client_protocol.h"

class Sender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<InputCmd>& client_queue;

public:
    void run() override;

    void stop() override;

    Sender(ClientProtocol& protocol, Queue<InputCmd>& client_queue);
};

#endif
