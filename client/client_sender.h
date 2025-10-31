#ifndef CLIENT_SENDER_H
#define CLIENT_SENDER_H

#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"

#include "client_protocol.h"

class ClientSender: public Thread {
private:
    ClientProtocol& protocol;
    Queue<InputCmd>& client_queue;

public:
    void run() override;

    void stop() override;

    ClientSender(ClientProtocol& protocol, Queue<InputCmd>& client_queue);
};

#endif
