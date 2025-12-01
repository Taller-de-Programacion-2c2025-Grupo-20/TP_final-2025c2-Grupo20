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
#include "../common/constants.h"
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

struct CarInfo {
    uint8_t player_id;
    CarType type;
    float next_race_time_penalty;

    CarInfo(uint8_t player_id, CarType type, float next_race_time_penalty) : player_id(player_id), type(type), next_race_time_penalty(next_race_time_penalty) {}
};

class Gameloop: public Thread {
private:
    Queue<InputCmd>& gameloop_queue;
    QueuesMonitor& clients_queues;

    YAML::Node map_data;

    std::unordered_map<uint8_t, std::unique_ptr<Car>> clients_cars;
    std::vector<std::unique_ptr<Wall>> world_walls;
    std::unordered_map<int, std::unique_ptr<Checkpoint>> world_checkpoints;
    std::vector<PlayerPos> cars_inital_pos;
    
    std::map<int, std::vector<PlayerRaceInfo>> races_results;
    std::vector<PlayerRaceInfo> not_finished_results;
    uint8_t next_finish_position;

    std::unordered_map<uint8_t, float> clients_acumulated_time;

    std::vector<CarInfo> deleted_cars;

    int current_race_number;

    b2World world;
    CollisionsListener collision_listener;

    std::chrono::steady_clock::time_point start_time;

    void addCarWithTimePenalty(uint8_t client_id, const CarType& car_type, float time_penalty);

    void handleInput(const InputCmd& input);

    void loadWalls();
    void loadCheckpoints(int race_number);
    void loadInitialPos(int race_number);

    void readUsersInput();

    float getCurrentCheckpointHintAngle(const b2Vec2& car_pos, float car_angle,
                                        const b2Vec2& checkpoint_pos);

    GameStateDTO getCurrentGameState(const float elapsed_time);

    void updatePhysics(const double& rate);

    std::chrono::_V2::steady_clock::time_point keepLoopRate(std::chrono::steady_clock::time_point t1, const double& rate);

    void removeClientsCars(float elapsed_time);
    
    void registerFinish(uint8_t player_id, float elapsed_time, float penalty_seconds);
    void registerDestroy(uint8_t player_id, float penalty_seconds);
    void registerTimeout();

    void resetCars();
    void moveToNextRace();
    void logRaceResults() const;

    bool raceEnded(float elapsed_time);
    void endRace();
    void logFinalResults() const;
    GameStateDTO gameEndedGamestate();

    bool queue_closed = false;

    YAML::Node findRaceGroup(int race_number);

public:
    void addCar(uint8_t client_id, const CarType& car_type);
    void loadMapData(const std::string& map_name);

    const std::map<int, std::vector<PlayerRaceInfo>>& racesResults() { return races_results; }
    const std::unordered_map<uint8_t, float>& clientsTime() { return clients_acumulated_time; }

    void run() override;

    void stop() override;

    Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues);

    ~Gameloop();
};


#endif
