#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

#include <Box2D/Box2D.h>
#include <yaml-cpp/yaml.h>

#include "../common/clientCommand.h"
#include "../common/queue.h"
#include "../common/thread.h"
#include "world_entities/car.h"
#include "world_entities/checkpoint.h"
#include "world_entities/wall.h"

#include "collisions_listener.h"
#include "queues_monitor.h"

struct PlayerPos {
    float x, y;

    PlayerPos(float x, float y): x(x), y(y) {}
};

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;

    std::unordered_map<uint8_t, std::unique_ptr<Car>> clients_cars;
    std::vector<std::unique_ptr<Wall>> world_walls;
    std::unordered_map<int, std::unique_ptr<Checkpoint>> world_checkpoints;
    std::vector<PlayerPos> cars_inital_pos;

    b2World world;
    CollisionsListener collision_listener;

    std::chrono::steady_clock::time_point start_time;

    std::mutex mutex;

    void handleInput(const InputCmd& input);

    void loadWalls(const YAML::Node& map_data);
    void loadCheckpoints(const YAML::Node& map_data);
    void loadInitialPos(const YAML::Node& map_data);

    void loadMapData();

    void readUsersInput();

    float getCurrentCheckpointHintAngle(const b2Vec2& car_pos, float car_angle,
                                        const b2Vec2& checkpoint_pos);

    GameStateDTO getCurrentGameState(const float elapsed_time);

public:
    void addCar(uint8_t client_id);

    void run() override;

    void stop() override;

    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues);
};


#endif
