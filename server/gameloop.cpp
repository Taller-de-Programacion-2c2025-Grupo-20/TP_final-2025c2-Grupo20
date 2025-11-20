#include "gameloop.h"
#include <chrono>
#include <string>
#include <thread>
#include <yaml-cpp/yaml.h>
#include <iostream>

#include "../common/constants.h"
#include "../common/queue.h"

const float timeStep = 1.f/60.f;
const float PIXELS_PER_METER = 16.0f;

Gameloop::Gameloop(Queue<InputCmd>& gameloop_queue,
                   QueuesMonitor& clients_queues,
                   b2World& world)
    : gameloop_queue(gameloop_queue),
      clients_queues(clients_queues),
      world(world)
{
    world.SetContactListener(&collisions_listener);
    loadMapData();
}

void Gameloop::handleInput(const InputCmd& input) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = clients_cars.find(input.player_id);
    if (it != clients_cars.end()) {
        it->second->handleInput(input);
    }
}

void Gameloop::add_player(uint8_t player_id) {

    b2Vec2 spawn_position(5.f + player_id, 5.f);

    auto car = std::make_unique<Car>(world, spawn_position);

    {
        std::lock_guard<std::mutex> lock(mutex);
        clients_cars.emplace(player_id, std::move(car));
    }

    std::cout << "Auto creado para jugador " << (int)player_id << std::endl;
}

/* ========================= MAP LOADING ========================== */

void Gameloop::loadWalls(const YAML::Node& map_data) {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "Paredes") {
            for (const auto& obj: layer["objects"]) {
                float x = (obj["x"].as<float>() + obj["width"].as<float>() / 2) / PIXELS_PER_METER;
                float y = (obj["y"].as<float>() + obj["height"].as<float>() / 2) / PIXELS_PER_METER;
                float w = (obj["width"].as<float>() / 2) / PIXELS_PER_METER;
                float h = (obj["height"].as<float>() / 2) / PIXELS_PER_METER;

                world_walls.push_back(std::make_unique<Wall>(world, b2Vec2(x, y), w, h));
            }
        }
    }
    std::cout << "Paredes cargadas.\n";
}

void Gameloop::loadCheckpoints(const YAML::Node& map_data) {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "Checkpoints") {
            for (const auto& obj: layer["objects"]) {

                float x = (obj["x"].as<float>() + obj["width"].as<float>() / 2) / PIXELS_PER_METER;
                float y = (obj["y"].as<float>() + obj["height"].as<float>() / 2) / PIXELS_PER_METER;
                float w = (obj["width"].as<float>() / 2) / PIXELS_PER_METER;
                float h = (obj["height"].as<float>() / 2) / PIXELS_PER_METER;

                int id = -1;

                if (obj["properties"]) {
                    for (const auto& p : obj["properties"]) {
                        if (p["name"].as<std::string>() == "ID") {
                            id = p["value"].as<int>();
                        }
                    }
                }

                world_checkpoints.push_back(
                    std::make_unique<Checkpoint>(world, b2Vec2(x, y), w, h, id)
                );
            }
        }
    }
    std::cout << "Checkpoints cargados.\n";
}

void Gameloop::loadInitialPos(const YAML::Node& map_data) {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "PosIniciales") {
            for (const auto& obj: layer["objects"]) {
                float x = (obj["x"].as<float>() + obj["width"].as<float>() / 2) / PIXELS_PER_METER;
                float y = (obj["y"].as<float>() + obj["height"].as<float>() / 2) / PIXELS_PER_METER;

                cars_initial_pos.emplace_back(x, y);
            }
        }
    }
    std::cout << "Posiciones iniciales cargadas.\n";
}

void Gameloop::loadMapData() {
    YAML::Node map_data =
            YAML::LoadFile(std::string(MAPS_DATA_PATH) + "liberty_city.yaml");

    loadWalls(map_data);
    loadCheckpoints(map_data);
    loadInitialPos(map_data);

    std::cout << "Datos del mapa cargados.\n";
}

/* ========================= GAME LOOP ========================== */

void Gameloop::readUsersInput() {
    InputCmd input;
    while (gameloop_queue.try_pop(input)) {
        handleInput(input);
    }
}

float Gameloop::getCurrentCheckpointHintAngle(const b2Vec2& pos, float angle,
                                              const b2Vec2& chk) {
    b2Vec2 dir = chk - pos;
    float chk_angle = std::atan2(dir.y, dir.x);

    float diff = chk_angle - angle;

    while (diff > M_PI) diff -= 2 * M_PI;
    while (diff < -M_PI) diff += 2 * M_PI;

    return diff;
}

GameStateDTO Gameloop::getCurrentGameState(float elapsed_time) {
    GameStateDTO state;
    state.car_count = clients_cars.size();
    state.elapsed_time = elapsed_time;

    for (auto& p : clients_cars) {
        auto& car = *(p.second);

        PlayerState ps;

        ps.player_id = p.first;
        ps.state = ServerState(car.position().x, car.position().y,
                               car.angle(), car.getSpeed());
        ps.health = car.health();

        int next = car.nextCheckpointId();
        if (next >= 0 && next < (int)world_checkpoints.size()) {
            auto& chk = world_checkpoints[next];
            ps.next_checkpoint_position_x = chk->position().x;
            ps.next_checkpoint_position_y = chk->position().y;
            ps.next_checkpoint_hint =
                getCurrentCheckpointHintAngle(car.position(), car.angle(), chk->position());
        }

        state.players.push_back(ps);
    }

    return state;
}

void Gameloop::run() {
    using clock = std::chrono::steady_clock;
    const std::chrono::duration<double> rate(1.0 / 60.0);

    start_time = clock::now();
    auto t1 = start_time;

    while (should_keep_running()) {

        try {
            readUsersInput();
        } catch (const ClosedQueue&) {
            break;
        }

        {
            std::lock_guard<std::mutex> lock(mutex);
            for (auto& p : clients_cars) {
                p.second->updateCarPhysics();
            }
        }

        world.Step(timeStep, 6, 2);

        float elapsed = std::chrono::duration<float>(clock::now() - start_time).count();

        GameStateDTO gs;
        {
            std::lock_guard<std::mutex> lock(mutex);
            gs = getCurrentGameState(elapsed);
        }

        clients_queues.broadcast(gs);

        auto t2 = clock::now();
        auto rest = rate - (t2 - t1);
        if (rest > std::chrono::duration<double>(0))
            std::this_thread::sleep_for(rest);

        t1 = clock::now();
    }

    std::cout << "Gameloop terminado.\n";
}

void Gameloop::stop() {
    Thread::stop();
    gameloop_queue.close();
}
