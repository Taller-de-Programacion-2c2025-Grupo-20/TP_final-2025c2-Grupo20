#ifndef GAMELOOP_H_
#define GAMELOOP_H_

#include <map>
#include <mutex>
#include <memory>
#include <vector>
#include <stdint.h>
#include <Box2D/Box2D.h>
#include <yaml-cpp/yaml.h> 

#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/clientCommand.h"
#include "../common/gameState.h"
#include "queues_monitor.h"
#include "world_entities/car.h"
#include "world_entities/wall.h"             
#include "world_entities/checkpoint.h"      
#include "collisions_listener.h" 

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;
    b2World& world;
    
    std::mutex mutex; 
    std::map<uint8_t, std::unique_ptr<Car>> clients_cars;

    std::vector<std::unique_ptr<Wall>> world_walls;
    std::vector<std::unique_ptr<Checkpoint>> world_checkpoints;
    CollisionsListener collisions_listener;
    
    std::chrono::steady_clock::time_point start_time;

    void handleInput(const InputCmd& input);
    void loadWalls(const YAML::Node& map_data);
    void loadMapData();
    void readUsersInput();
    GameStateDTO getCurrentGameState();

public:
    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues, b2World& world);

    void run() override;
    void stop() override;

    void add_player(uint8_t player_id);
};

#endif