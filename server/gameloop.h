#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <unordered_map>

#include <Box2D/Box2D.h>

#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"

#include "queues_monitor.h"

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;

    b2World world;

public:
    void run() override;

    void stop() override;

    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues);
};


#endif
