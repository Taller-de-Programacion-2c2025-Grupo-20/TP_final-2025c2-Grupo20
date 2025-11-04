#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <unordered_map>

#include <Box2D/Box2D.h>

#include "car.h"
#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"

#include "queues_monitor.h"

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;

    std::unordered_map<int, Car> clients_cars;

    b2World world;

    void handleInput(const InputCmd& input);

    void load_map();

public:
    void run() override;

    void stop() override;

    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues);
};


#endif
