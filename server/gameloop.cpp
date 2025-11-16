#include "gameloop.h"

#include <chrono>
#include <string>
#include <thread>

#include "../common/constants.h"
#include "../common/queue.h"

const float timeStep = 1.f / 60.f;
const float PIXELS_PER_METER = 16.0f;

void Gameloop::handleInput(const InputCmd& input) {
    auto it = clients_cars.find(input.player_id);

    if (it == clients_cars.end()) {
        return;
    }

    it->second->handleInput(input);

    std::cout << "Pos del auto en x: " << it->second->position().x
              << " Pos del auto en y: " << it->second->position().y << "\n"
              << "Angulo: " << it->second->angle() << "\n";
}

void Gameloop::addCar(uint8_t client_id) {
    mutex.lock();
    PlayerPos car_initial_pos = cars_inital_pos.front();
    cars_inital_pos.erase(cars_inital_pos.begin());

    clients_cars.emplace(
            client_id, std::make_unique<Car>(world, b2Vec2(car_initial_pos.x, car_initial_pos.y)));
    mutex.unlock();
}

void Gameloop::loadWalls(const YAML::Node& map_data) {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "Paredes") {
            for (const auto& obj: layer["objects"]) {
                float x_pixels = obj["x"].as<float>();
                float y_pixels = obj["y"].as<float>();
                float width_pixels = obj["width"].as<float>();
                float height_pixels = obj["height"].as<float>();

                float x_meters = (x_pixels + width_pixels / 2) / PIXELS_PER_METER;
                float y_meters = (y_pixels + height_pixels / 2) / PIXELS_PER_METER;

                float width_meters = (width_pixels / 2) / PIXELS_PER_METER;
                float height_meters = (height_pixels / 2) / PIXELS_PER_METER;

                world_walls.push_back(std::make_unique<Wall>(world, b2Vec2(x_meters, y_meters),
                                                             width_meters, height_meters));
            }
        }
    }

    std::cout << "Termino carga de paredes\n";
}

void Gameloop::loadCheckpoints(const YAML::Node& map_data) {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "Checkpoints") {
            for (const auto& obj: layer["objects"]) {
                float x_pixels = obj["x"].as<float>();
                float y_pixels = obj["y"].as<float>();
                float width_pixels = obj["width"].as<float>();
                float height_pixels = obj["height"].as<float>();

                float x_meters = (x_pixels + width_pixels / 2) / PIXELS_PER_METER;
                float y_meters = (y_pixels + height_pixels / 2) / PIXELS_PER_METER;
                float width_meters = (width_pixels / 2) / PIXELS_PER_METER;
                float height_meters = (height_pixels / 2) / PIXELS_PER_METER;

                int checkpoint_id = -1;

                if (obj["properties"]) {
                    for (const auto& prop : obj["properties"]) {
                        if (prop["name"].as<std::string>() == "ID") {
                            checkpoint_id = prop["value"].as<int>();
                            break;
                        }
                    }
                }

                world_checkpoints.emplace(
                        checkpoint_id, std::make_unique<Checkpoint>(world, b2Vec2(x_meters, y_meters),
                                                         width_meters, height_meters, checkpoint_id));
            }
        }
    }

    std::cout << "Termino carga de checkpoints\n";
}

void Gameloop::loadInitialPos(const YAML::Node& map_data) {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "PosIniciales") {
            for (const auto& obj: layer["objects"]) {
                float x_pixels = obj["x"].as<float>();
                float y_pixels = obj["y"].as<float>();
                float width_pixels = obj["width"].as<float>();
                float height_pixels = obj["height"].as<float>();

                float x_meters = (x_pixels + width_pixels / 2) / PIXELS_PER_METER;
                float y_meters = (y_pixels + height_pixels / 2) / PIXELS_PER_METER;

                cars_inital_pos.push_back(PlayerPos(x_meters, y_meters));
            }
        }
    }

    std::cout << "Termino carga de posiciones iniciales\n";
}

void Gameloop::loadMapData() {
    YAML::Node map_data =
            YAML::LoadFile("../liberty_city.yaml");

    loadWalls(map_data);
    loadCheckpoints(map_data);
    loadInitialPos(map_data);

    std::cout << "Termino carga de datos del mapa\n";
}

void Gameloop::readUsersInput() {
    InputCmd input;
    while (gameloop_queue.try_pop(input)) {
        handleInput(input);
        std::cout << "Player id: " << static_cast<int>(input.player_id) << "\n";
    }
}

float Gameloop::getCurrentCheckpointHintAngle(const b2Vec2& car_pos, float car_angle,
                                              const b2Vec2& checkpoint_pos) {
    b2Vec2 hint_dir = checkpoint_pos - car_pos;
    float checkpoint_angle = std::atan2(hint_dir.y, hint_dir.x);

    float direction = checkpoint_angle - car_angle;

    while (direction > M_PI) {
        direction -= 2 * M_PI;
    }
    while (direction < -M_PI) {
        direction += 2 * M_PI;
    }

    return direction;
}

GameStateDTO Gameloop::getCurrentGameState(const float elapsed_time) {
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

        if ((size_t)current_client_car->nextCheckpointId() < world_checkpoints.size()) {
            int next_checkpoint_id = current_client_car->nextCheckpointId();
            current_player_state.next_checkpoint_position_x =
                    world_checkpoints[next_checkpoint_id]->position().x;
            current_player_state.next_checkpoint_position_y =
                    world_checkpoints[next_checkpoint_id]->position().y;
            current_player_state.next_checkpoint_hint = getCurrentCheckpointHintAngle(
                    current_client_car->position(), current_client_car->angle(),
                    world_checkpoints[next_checkpoint_id]->position());
        } else {
            current_player_state.next_checkpoint_position_x = 0;
            current_player_state.next_checkpoint_position_y = 0;
            current_player_state.next_checkpoint_hint = 0;
        }

        current_state.players.push_back(current_player_state);
    }

    current_state.elapsed_time = elapsed_time;

    return current_state;
}

void Gameloop::run() {

    using clock = std::chrono::steady_clock;
    const double rate = 1.0 / 60.0;
    auto t1 = clock::now();

    start_time = t1;

    while (should_keep_running()) {

        try {
            readUsersInput();
        } catch (const ClosedQueue&) {
            std::cout << "Gameloop: cola cerrada, saliendo.\n";
            break;
        }

        for (auto& pair: clients_cars) {
            pair.second->updateCarPhysics();
        }
        world.Step(rate, 6, 2);

        auto t2 = clock::now();
        double elapsed = std::chrono::duration<double>(t2 - t1).count();

        double rest = rate - elapsed;

        if (rest < 0) {
            double behind = -rest;

            double adjust = rate - fmod(behind, rate);
            double lost = behind + adjust;

            auto lost_dur = std::chrono::duration_cast<clock::duration>(
                    std::chrono::duration<double>(lost));

            t1 += lost_dur;

        } else {

            std::this_thread::sleep_for(std::chrono::duration<double>(rest));

            auto rate_dur = std::chrono::duration_cast<clock::duration>(
                    std::chrono::duration<double>(rate));

            t1 += rate_dur;
        }

        float elapsed_time = std::chrono::duration<float>(clock::now() - start_time).count();
        GameStateDTO state = getCurrentGameState(elapsed_time);
        clients_queues.broadcast(state);
    }

    std::cout << "Gameloop terminado.\n";
}

void Gameloop::stop() {
    Thread::stop();
    gameloop_queue.close();
}

Gameloop::Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues):
        gameloop_queue(gameloop_queue), clients_queues(clients_queues), world(b2Vec2(0, 0), true) {
    world.SetContactListener(&collision_listener);
    loadMapData();
}
