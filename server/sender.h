#ifndef SENDER_H
#define SENDER_H

#include <mutex>
#include <atomic>
#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/gameState.h"
#include "server_protocol.h"

class Sender: public Thread {
private:
    ServerProtocol& protocol;
    
    Queue<GameStateDTO>* client_queue;
    std::mutex mtx; 

public:
    void run() override;
    void stop() override;

    Sender(ServerProtocol& protocol);

    void set_state_queue(Queue<GameStateDTO>& new_queue);
};

#endif