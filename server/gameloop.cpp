#include "gameloop.h"
#include <chrono>
#include <thread>
#include <yaml-cpp/yaml.h>
#include <iostream>

#include "../common/constants.h"
#include "../common/queue.h"

const float timeStep = 1.f/60.f;
const float PIXELS_PER_METER = 16.0f;

Gameloop::Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues, b2World& world):
        gameloop_queue(gameloop_queue), 
        clients_queues(clients_queues), 
        world(world)
{
    world_checkpoints.push_back(std::make_unique<Checkpoint>(
                    world, b2Vec2(5.f, 10.f), 2, 2, 0
                ));
    world.SetContactListener(&collisions_listener);
    loadMapData();
}


void Gameloop::handleInput(const InputCmd& input) {
    auto it = clients_cars.find(input.player_id);
    if (it == clients_cars.end()) {
        return;
    }
    it->second->handleInput(input);
}

void Gameloop::add_player(uint8_t player_id) {
    
    b2Vec2 spawn_position(5.f + player_id, 5.f); 
    auto new_car = std::make_unique<Car>(world, spawn_position);
    clients_cars.insert(std::make_pair(player_id, std::move(new_car)));
    
    std::cout << "Auto para jugador " << (int)player_id << " creado en la simulación." << std::endl;
}

void Gameloop::loadWalls(const YAML::Node& map_data) {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "Colisiones") {
            for (const auto& obj: layer["objects"]) {
                float x_pixels = obj["x"].as<float>();
                float y_pixels = obj["y"].as<float>();
                float width_pixels = obj["width"].as<float>();
                float height_pixels = obj["height"].as<float>();
                float x_meters = (x_pixels + width_pixels / 2) / PIXELS_PER_METER;
                float y_meters = (y_pixels + height_pixels / 2) / PIXELS_PER_METER;
                float width_meters = (width_pixels / 2) / PIXELS_PER_METER;
                float height_meters = (height_pixels / 2) / PIXELS_PER_METER;

                world_walls.push_back(std::make_unique<Wall>(
                    world, b2Vec2(x_meters, y_meters), width_meters, height_meters
                ));
            }
        }
    }
}

void Gameloop::loadMapData() {
    try {
        YAML::Node map_data = YAML::LoadFile("liberty.yaml");
        loadWalls(map_data);
    } catch (const std::exception& e) {
        std::cerr << "ERROR: No se pudo cargar el mapa ../liberty.yaml: " << e.what() << std::endl;
        throw; 
    }
}
void Gameloop::readUsersInput() {
    InputCmd input;
    while (gameloop_queue.try_pop(input)) {
        handleInput(input);
    }
}

GameStateDTO Gameloop::getCurrentGameState() {
    GameStateDTO current_state;
    current_state.car_count = clients_cars.size();

    for (auto& pair: clients_cars) {
        auto& current_client_car = pair.second;
        PlayerState current_player_state;
        current_player_state.player_id = pair.first;
        current_player_state.state =
                ServerState(current_client_car->position().x, current_client_car->position().y,
                            current_client_car->angle(), current_client_car->getSpeed());
        current_player_state.health = current_client_car->health();
        current_state.players.push_back(current_player_state);
    }
    return current_state;
}

void Gameloop::run() {
    using clock = std::chrono::steady_clock;
    const std::chrono::duration<double> rate(1.0 / 60.0);
    start_time = clock::now();
    auto t1 = start_time;

    while (should_keep_running()) {
        try {
            {
                std::lock_guard<std::mutex> lock(mutex);
                readUsersInput(); 
                for (auto& pair: clients_cars) {
                    pair.second->updateCarPhysics();
                }
            }

            world.Step(timeStep, 6, 2);

            GameStateDTO current_state;
            {
                std::lock_guard<std::mutex> lock(mutex);
                
                current_state = getCurrentGameState();
            }

            current_state.elapsed_time = std::chrono::duration<float>(clock::now() - start_time).count();
            clients_queues.broadcast(current_state);


            auto t2 = clock::now();
            auto rest = rate - (t2 - t1);

            if (rest < std::chrono::duration<double>(0)) {
                auto behind = -rest;
                auto lost =
                        behind +
                        (rate - std::chrono::duration<double>(fmod(behind.count(), rate.count())));
                t1 += std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                        std::chrono::duration<double>(lost.count()));
            }

            std::this_thread::sleep_for(rest);
            t1 += std::chrono::duration_cast<std::chrono::steady_clock::duration>(rate);

        } catch (const ClosedQueue&) {
            break; 
        }
    }
}

void Gameloop::stop() {
    Thread::stop();
    gameloop_queue.close();
}