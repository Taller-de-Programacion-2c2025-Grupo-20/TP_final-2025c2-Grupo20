#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>

#include "../common_src/queue.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"

#include "client_handler.h"
#include "queues_monitor.h"

class Acceptor: public Thread {
private:
    Socket skt;
    Queue<Instruction>& gameloop_queue;
    QueuesMonitor& clients_queues;
    std::list<ClientHandler*> client_handlers_list;

    void reap();
    void clear();

public:
    void run() override;

    void stop() override;

    explicit Acceptor(const char* port, Queue<Instruction>& gameloop_queue,
                      QueuesMonitor& clients_queues);
};

#endif
