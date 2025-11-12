#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <memory>
#include <atomic>
#include "../common/queue.h"
#include "../common/socket.h"
#include "../common/thread.h"
#include "client_handler.h"

class Acceptor : public Thread {
private:
    Socket& listener; 
    
    Queue<std::unique_ptr<ClientHandler>>& new_clients_queue;

public:
    void run() override;
    void stop() override;

    explicit Acceptor(Socket& listener,
                      Queue<std::unique_ptr<ClientHandler>>& new_clients_queue);
};

#endif