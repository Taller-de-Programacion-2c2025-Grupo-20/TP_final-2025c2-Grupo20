#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <memory>
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

struct PlayerRaceInfo {
    uint8_t player_id;
    uint8_t position;
    float finish_time;    
    bool finished;
    bool destroyed;
    bool timed_out;
};

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;

    std::unordered_map<uint8_t, std::unique_ptr<Car>> clients_cars;
    std::vector<std::unique_ptr<Wall>> world_walls;
    std::unordered_map<int, std::unique_ptr<Checkpoint>> world_checkpoints;
    std::vector<PlayerPos> cars_inital_pos;
    
    std::vector<PlayerRaceInfo> race_results;
    std::vector<PlayerRaceInfo> not_finished_results;
    uint8_t next_finish_position;

    b2World world;
    CollisionsListener collision_listener;

    std::chrono::steady_clock::time_point start_time;

    void handleInput(const InputCmd& input);

    void loadWalls(const YAML::Node& map_data);
    void loadCheckpoints(const YAML::Node& map_data);
    void loadInitialPos(const YAML::Node& map_data);

    void readUsersInput();

    float getCurrentCheckpointHintAngle(const b2Vec2& car_pos, float car_angle,
                                        const b2Vec2& checkpoint_pos);

    GameStateDTO getCurrentGameState(const float elapsed_time);

    void updatePhysics(const double& rate);

    std::chrono::_V2::steady_clock::time_point keepLoopRate(std::chrono::steady_clock::time_point t1, const double& rate);

    void removeClientsCars(float elapsed_time);
    
    void registerFinish(uint8_t player_id, float elapsed_time);
    void registerDestroy(uint8_t player_id, float elapsed_time);
    void registerTimeout(float elapsed_time);

    void logRaceResults() const;

    bool gameEnded(float elapsed_time);

    bool queue_closed = false;

public:
    void addCar(uint8_t client_id, const CarType& car_type);
    void loadMapData(const std::string& map_name);

    const std::vector<PlayerRaceInfo>& getRaceResults() const { return race_results; }

    void run() override;

    void stop() override;

    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues);

    ~Gameloop();
};


#endif
