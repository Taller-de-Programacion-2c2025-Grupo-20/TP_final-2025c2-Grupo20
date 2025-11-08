#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <mutex>
#include <unordered_map>

#include <Box2D/Box2D.h>
#include <yaml-cpp/yaml.h>

#include "../common/clientCommand.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "car.h"
#include "collisions_listener.h"
#include "queues_monitor.h"

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;

    std::unordered_map<uint8_t, Car> clients_cars;

    b2World world;
    CollisionsListener collision_listener;

    std::mutex mutex;

    void handleInput(const InputCmd& input);

    void loadWalls(const YAML::Node& map_data);

    void loadMapData();

    void readUsersInput();

    GameStateDTO getCurrentGameState();

public:
    void addCar(uint8_t client_id);

    void run() override;

    void stop() override;

    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues);
};


#endif
