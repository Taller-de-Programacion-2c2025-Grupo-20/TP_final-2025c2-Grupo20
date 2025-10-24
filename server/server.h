#ifndef SERVER_H
#define SERVER_H

#include "../common_src/queue.h"
#include "../common_src/socket.h"

#include "client_handler.h"
#include "queues_monitor.h"

class Server {
private:
    const char* port;

    QueuesMonitor clients_queues;
    Queue<Instruction> gameloop_queue;

public:
    int run();

    explicit Server(const char* port);

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    ~Server();
};

#endif
