#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <list>

#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"

#include "client_handler.h"
#include "queues_monitor.h"

#include "gameloop.h"

class Acceptor: public Thread {
private:
    Socket skt;
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;
    std::list<ClientHandler*> client_handlers_list;

    Gameloop& gameloop;

    void reap();
    void clear();

public:
    void run() override;

    void stop() override;

    explicit Acceptor(const char* port, Queue<InputCmd>& gameloop_queue,
                      QueuesMonitor& clients_queues, Gameloop& gameloop);
};

#endif
