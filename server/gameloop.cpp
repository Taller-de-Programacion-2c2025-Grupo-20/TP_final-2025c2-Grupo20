#include "gameloop.h"

#include <chrono>
#include <string>
#include <thread>

#include "../common/config.h"
#include "../common/constants.h"
#include "../common/queue.h"

void Gameloop::handleInput(const InputCmd& input) {
    auto it = clients_cars.find(input.player_id);

    if (it == clients_cars.end()) {
        return;
    }

    it->second->handleInput(input);
}

void Gameloop::addCar(uint8_t client_id, const CarType& car_type) {
    PlayerPos car_initial_pos = cars_inital_pos.front();
    cars_inital_pos.erase(cars_inital_pos.begin());

    clients_cars.emplace(
            client_id, std::make_unique<Car>(world, b2Vec2(car_initial_pos.x, car_initial_pos.y), car_type, 0.0));
 
    std::cout << "Auto creado para jugador " << (int)client_id << std::endl;
}

void Gameloop::addCarWithTimePenalty(uint8_t client_id, const CarType& car_type, float time_penalty) {
    PlayerPos car_initial_pos = cars_inital_pos.front();
    cars_inital_pos.erase(cars_inital_pos.begin());

    clients_cars.emplace(
            client_id, std::make_unique<Car>(world, b2Vec2(car_initial_pos.x, car_initial_pos.y), car_type, time_penalty));
 
    std::cout << "Auto con penalizacion creado para jugador " << (int)client_id << std::endl;
}

/* ========================= MAP LOADING ========================== */

void Gameloop::loadWalls() {
    for (const auto& layer: map_data["layers"]) {
        if (layer["name"].as<std::string>() == "Paredes") {
            for (const auto& obj: layer["objects"]) {
                float x_pixels = obj["x"].as<float>();
                float y_pixels = obj["y"].as<float>();
                float width_pixels = obj["width"].as<float>();
                float height_pixels = obj["height"].as<float>();

                float x_meters = (x_pixels + width_pixels / 2) / Config::get().PIXELS_PER_METER();
                float y_meters = (y_pixels + height_pixels / 2) / Config::get().PIXELS_PER_METER();

                float width_meters = (width_pixels / 2) / Config::get().PIXELS_PER_METER();
                float height_meters = (height_pixels / 2) / Config::get().PIXELS_PER_METER();

                world_walls.push_back(std::make_unique<Wall>(world, b2Vec2(x_meters, y_meters),
                                                             width_meters, height_meters));
            }
        }
    }

    std::cout << "Termino carga de paredes\n";
}

YAML::Node Gameloop::findRaceGroup(int race_number) {
    std::string target = std::to_string(race_number);

    for (const auto& layer : map_data["layers"]) {
        if (layer["name"].as<std::string>() == target) {
            return layer;
        }
    }

    throw std::runtime_error("No existe el grupo de carrera " + target);
}

void Gameloop::loadCheckpoints(int race_number) {
    YAML::Node grupo = findRaceGroup(race_number);

    for (const auto& sublayer : grupo["layers"]) {
        if (sublayer["name"].as<std::string>() == "Checkpoints") {

            for (const auto& obj : sublayer["objects"]) {
                float x_pixels = obj["x"].as<float>();
                float y_pixels = obj["y"].as<float>();
                float width_pixels = obj["width"].as<float>();
                float height_pixels = obj["height"].as<float>();

                float x_meters = (x_pixels + width_pixels / 2) / Config::get().PIXELS_PER_METER();
                float y_meters = (y_pixels + height_pixels / 2) / Config::get().PIXELS_PER_METER();
                float width_meters = (width_pixels / 2) / Config::get().PIXELS_PER_METER();
                float height_meters = (height_pixels / 2) / Config::get().PIXELS_PER_METER();

                int checkpoint_id = -1;
                if (obj["properties"]) {
                    for (const auto& prop: obj["properties"]) {
                        if (prop["name"].as<std::string>() == "ID") {
                            checkpoint_id = prop["value"].as<int>();
                            break;
                        }
                    }
                }

                world_checkpoints.emplace(
                    checkpoint_id,
                    std::make_unique<Checkpoint>(
                        world, b2Vec2(x_meters, y_meters),
                        width_meters, height_meters, checkpoint_id
                    )
                );
            }
        }
    }

    std::cout << "Termino carga de checkpoints para carrera "
              << race_number << "\n";
}

void Gameloop::loadInitialPos(int race_number) {
    YAML::Node grupo = findRaceGroup(race_number);

    for (const auto& sublayer : grupo["layers"]) {
        if (sublayer["name"].as<std::string>() == "PosIniciales") {

            for (const auto& obj : sublayer["objects"]) {
                float x_pixels = obj["x"].as<float>();
                float y_pixels = obj["y"].as<float>();
                float width_pixels = obj["width"].as<float>();
                float height_pixels = obj["height"].as<float>();

                float x_meters = (x_pixels + width_pixels / 2) / Config::get().PIXELS_PER_METER();
                float y_meters = (y_pixels + height_pixels / 2) / Config::get().PIXELS_PER_METER();

                cars_inital_pos.emplace_back(x_meters, y_meters);
            }
        }
    }

    std::cout << "Termino carga de posiciones iniciales para carrera "
              << race_number << "\n";
}

void Gameloop::loadMapData(const std::string& map_name) {
    world_walls.clear();
    world_checkpoints.clear();
    cars_inital_pos.clear();

    map_data = YAML::LoadFile(std::string(MAPS_DATA_PATH) + map_name);

    loadWalls();
    loadCheckpoints(1);
    loadInitialPos(1);

    std::cout << "Gameloop: Mapa " << map_name << " cargado correctamente.\n";
}

/* ========================= GAME LOOP ========================== */
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

        current_player_state.car_type = current_client_car->getCarType();
        current_player_state.applied_upgrades = current_client_car->applyedUpgrades();

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

        current_player_state.checkpoints_passed = static_cast<uint8_t>(current_client_car->nextCheckpointId());

        current_state.players.push_back(current_player_state);
    }

    current_state.elapsed_time = elapsed_time;

    current_state.is_running = 1;

    return current_state;
}

void Gameloop::updatePhysics(const double& rate) {
    for (auto& pair: clients_cars) {

        pair.second->updateCarPhysics();

    }
    world.Step(rate, 6, 2);
}

void Gameloop::registerFinish(uint8_t player_id, float elapsed_time, float penalty_seconds) {
    races_results[current_race_number].push_back(
            {player_id, next_finish_position++, elapsed_time + penalty_seconds, true, false, false});
}

void Gameloop::registerDestroy(uint8_t player_id, float penalty_seconds) {
    not_finished_results.push_back(
            {player_id, 0, Config::get().RACE_DURATION_SECONDS() + penalty_seconds, false, true, false});
}

void Gameloop::registerTimeout() {
    for (auto& car : clients_cars) {
        float penalty = car.second->timePenalty();
        not_finished_results.push_back(
                {car.first, 0, Config::get().RACE_DURATION_SECONDS() + penalty, false, false, true});
    }
}

void Gameloop::logRaceResults() const {
    std::cout << "===== Resultados de la carrera =====\n";

    auto it = races_results.find(current_race_number);
    if (it == races_results.end()) {
        std::cout << "No hay resultados registrados para la carrera "
                << current_race_number << "\n";
        return;
    }

    for (const auto& res : races_results.at(current_race_number)) {
        std::cout << "Jugador " << static_cast<int>(res.player_id) << ": ";
        if (res.finished) {
            std::cout << "posicion " << static_cast<int>(res.position)
                      << ", tiempo " << res.finish_time << "s\n";
        } else if (res.destroyed) {
            std::cout << "eliminado por destruccion en " << res.finish_time << "s\n";
        } else if (res.timed_out) {
            std::cout << "timeout, no llego en " << res.finish_time << "s\n";
        } else {
            std::cout << "estado desconocido en " << res.finish_time << "s\n";
        }
    }
    std::cout << "====================================\n";
}

void Gameloop::removeClientsCars(float elapsed_time) {
    const size_t total_checkpoints = world_checkpoints.size();

    for (auto it = clients_cars.begin(); it != clients_cars.end();) {
        const bool destroyed = it->second->health() == 0;
        const bool finished = (static_cast<size_t>(it->second->nextCheckpointId()) >= total_checkpoints);

        if (destroyed) {
            std::cout << "Eliminando auto del jugador con ID: " << static_cast<int>(it->first)
                      << " por vida 0\n";
            registerDestroy(it->first, it->second->timePenalty());

            deleted_cars.push_back(CarInfo(it->first, it->second->getCarType(), it->second->nextRaceTimePenalty()));

            it = clients_cars.erase(it);
            continue;
        }

        if (finished) {
            std::cout << "Jugador " << static_cast<int>(it->first)
                      << " completó el último checkpoint, sacando su auto del mapa.\n";
            registerFinish(it->first, elapsed_time, it->second->timePenalty());

            deleted_cars.push_back(CarInfo(it->first, it->second->getCarType(), it->second->nextRaceTimePenalty()));

            it = clients_cars.erase(it);
            continue;
        }

        ++it;
    }
}

void Gameloop::endRace() {

    logRaceResults();

    for (const auto& res : races_results[current_race_number]) {
        clients_acumulated_time[res.player_id] += res.finish_time;
    }
}

void Gameloop::logFinalResults() const {
    if (clients_acumulated_time.empty()) {
        return;
    }

    // 1) Ordenar jugadores por tiempo acumulado (ya calculado)
    std::vector<std::pair<uint8_t, float>> sorted(
        clients_acumulated_time.begin(),
        clients_acumulated_time.end()
    );

    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) {
                  return a.second < b.second;
              });

    std::cout << "===== Clasificacion final =====\n";

    int final_pos = 1;

    // 2) Mostrar info por jugador
    for (const auto& [player_id, total_time] : sorted) {
        std::cout << "Jugador " << (int)player_id
                  << " — Posición final " << final_pos++ << "\n";

        // 3) Mostrar las 3 carreras
        for (int race = 1; race <= 3; race++) {
            std::cout << "  Carrera " << race << ": ";

            auto it = races_results.find(race);
            if (it == races_results.end()) {
                std::cout << "sin datos\n";
                continue;
            }

            const auto& race_vec = it->second;

            // encontrar info del jugador en esta carrera
            auto rit = std::find_if(race_vec.begin(), race_vec.end(),
                                    [&](const PlayerRaceInfo& info) {
                                        return info.player_id == player_id;
                                    });

            if (rit == race_vec.end()) {
                std::cout << "no participó\n";
                continue;
            }

            const PlayerRaceInfo& r = *rit;

            // 4) Mostrar estado concreto
            if (r.finished) {
                std::cout << "terminó en posición "
                          << (int)r.position
                          << " con tiempo "
                          << r.finish_time << "s\n";
            } else if (r.destroyed) {
                std::cout << "DESTRUIDO\n";
            } else if (r.timed_out) {
                std::cout << "TIMEOUT\n";
            } else {
                std::cout << "estado desconocido\n";
            }
        }

        std::cout << "  Tiempo total acumulado: "
                  << total_time << "s\n\n";
    }

    // 5) Ganador
    std::cout << "Ganador: jugador "
              << (int)sorted.front().first << "\n";

    std::cout << "================================\n";
}



bool Gameloop::raceEnded(float elapsed_time) {
    if (elapsed_time >= MATCH_DURATION_SECONDS) {
        std::cout << "La partida alcanzó 10 minutos.\n";
        
        registerTimeout();

        return true;
    }

    if (clients_cars.empty()) {
        std::cout << "No quedan jugadores vivos o terminaron todos.\n";
        return true;
    }

    return false;
}

void Gameloop::resetCars() {

    std::vector<CarInfo> all_cars;

    for (auto& client_car : clients_cars) {
        all_cars.emplace_back( CarInfo(client_car.first, client_car.second->getCarType(), client_car.second->nextRaceTimePenalty()) );
    }

    for (auto& deleted_car : deleted_cars) {
        all_cars.emplace_back(deleted_car);
    }

    clients_cars.clear();
    deleted_cars.clear();

    for (auto& car : all_cars) {

        if (car.next_race_time_penalty != 0.0){
            addCarWithTimePenalty(car.player_id, car.type, car.next_race_time_penalty);
        } else {
            addCar(car.player_id, car.type);
        }

    }

}

void Gameloop::moveToNextRace() {
    next_finish_position = 1;

    world_checkpoints.clear();
    cars_inital_pos.clear();

    loadInitialPos(current_race_number);
    loadCheckpoints(current_race_number);
    
    resetCars();

}

std::chrono::_V2::steady_clock::time_point Gameloop::keepLoopRate(std::chrono::steady_clock::time_point t1, const double& rate) {
    using clock = std::chrono::steady_clock;
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

    return t1;
}

GameStateDTO Gameloop::gameEndedGamestate(){
    GameStateDTO current_state;
    current_state.car_count = 0;
    current_state.elapsed_time = 0;
    current_state.is_running = 0;
    return current_state;
}

void Gameloop::run() {

    const double rate = 1.0 / 60.0;

    while (should_keep_running() && (current_race_number <= Config::get().MAX_RACES() ) ) {

        if (current_race_number > 1) {
            moveToNextRace();
        }

        using clock = std::chrono::steady_clock;
        auto t1 = clock::now();

        start_time = t1;

        while (should_keep_running()) {

            try {
                readUsersInput();
            } catch (const ClosedQueue&) {
                std::cout << "Gameloop: cola cerrada, saliendo.\n";
                break;
            }

            updatePhysics(rate);

            float elapsed_time = std::chrono::duration<float>(clock::now() - start_time).count();
            removeClientsCars(elapsed_time);

            if (raceEnded(elapsed_time)) {
                races_results[current_race_number].insert(
                    races_results[current_race_number].end(),
                    not_finished_results.begin(),
                    not_finished_results.end()
                );
                not_finished_results.clear();
                break;
            }

            clients_queues.broadcast(getCurrentGameState(elapsed_time));

            t1 = keepLoopRate(t1, rate);
        }

        endRace();

        current_race_number++;
    }

    std::cout << "Gameloop terminado.\n";
    logFinalResults();

    std::cout << "Enviando mensaje de finalizacion de partida a los clientes...\n";
    clients_queues.broadcast(gameEndedGamestate());
}

void Gameloop::stop() {
    Thread::stop();
    gameloop_queue.close();
}

Gameloop::Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues):
        gameloop_queue(gameloop_queue), 
        clients_queues(clients_queues), 
        next_finish_position(1),
        current_race_number(1),
        world(b2Vec2(0, 0), true) {

            world.SetContactListener(&collision_listener);

}

Gameloop::~Gameloop() {

    clients_cars.clear();
    world_checkpoints.clear();
    world_walls.clear();

}
