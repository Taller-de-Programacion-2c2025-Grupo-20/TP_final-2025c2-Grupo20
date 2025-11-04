#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <unordered_map>

#include <Box2D/Box2D.h>
#include <mutex>

#include "car.h"
#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"

#include "queues_monitor.h"

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;

    std::unordered_map<uint8_t, Car> clients_cars;

    b2World world;

    std::mutex mutex;

    void handleInput(const InputCmd& input);

public:
    void addCar(uint8_t client_id);

    void run() override;

    void stop() override;

    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues);
};


#endif
