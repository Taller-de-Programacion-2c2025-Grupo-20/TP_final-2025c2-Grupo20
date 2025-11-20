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

struct PlayerPos {
    float x, y;
    PlayerPos(float x, float y): x(x), y(y) {}
};

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;
    b2World& world;

    std::mutex mutex;
    std::map<uint8_t, std::unique_ptr<Car>> clients_cars;

    std::vector<std::unique_ptr<Wall>> world_walls;
    std::vector<std::unique_ptr<Checkpoint>> world_checkpoints;
    std::vector<PlayerPos> cars_initial_pos;

    CollisionsListener collisions_listener;

    std::chrono::steady_clock::time_point start_time;

    void handleInput(const InputCmd& input);
    void loadWalls(const YAML::Node& map_data);
    void loadCheckpoints(const YAML::Node& map_data);
    void loadInitialPos(const YAML::Node& map_data);
    void loadMapData();
    void readUsersInput();

    float getCurrentCheckpointHintAngle(const b2Vec2& car_pos, float car_angle,
                                        const b2Vec2& checkpoint_pos);

    GameStateDTO getCurrentGameState(float elapsed_time);

public:
    Gameloop(Queue<InputCmd>& gameloop_queue,
             QueuesMonitor& clients_queues,
             b2World& world);

    void run() override;
    void stop() override;

    void add_player(uint8_t player_id);
};

#endif
