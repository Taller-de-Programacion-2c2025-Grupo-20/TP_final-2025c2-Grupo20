#include "gameloop.h"

#include <chrono>
#include <thread>

#include "../common/constants.h"
#include "../common/queue.h"

const float timeStep = 1.f/60.f;

void Gameloop::handleInput(const InputCmd& input) {
    auto it = clients_cars.find(input.player_id);

    if (it == clients_cars.end()) {
        return;
    }

    it->second.handleInput(input);

    std::cout << "Pos del auto en x: " << it->second.position().x
              << " Pos del auto en y: " << it->second.position().y << "\n"
              << "Angulo: " << it->second.angle() << "\n";
}

void Gameloop::run() {

    while (should_keep_running()) {
        try {
            InputCmd input;
            while (gameloop_queue.try_pop(input)) {
                handleInput(input);
                std::cout << "Player id: " << static_cast<int>(input.player_id) << "\n";
            }
            
            for (auto& pair : clients_cars) {
                auto& client_car = pair.second;
                client_car.updateCarPhysics();
            }

            world.Step(timeStep, 6, 2);

            GameStateDTO current_state;
            current_state.car_count = clients_cars.size();

            for (auto& pair : clients_cars) {
                auto& current_client_car = pair.second;
                
                PlayerState current_player_state;
                current_player_state.player_id = pair.first;
                current_player_state.state = ServerState(current_client_car.position().x, current_client_car.position().y, current_client_car.angle());
                current_player_state.health = 100;

                current_state.players.push_back(current_player_state);
            }

            clients_queues.broadcast(current_state);

            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        } catch (const ClosedQueue&) {
            break;
        }
    }
}

void Gameloop::stop() {
    Thread::stop();
    gameloop_queue.close();
}

Gameloop::Gameloop(Queue<InputCmd>& gameloop_queue, QueuesMonitor& clients_queues):
        gameloop_queue(gameloop_queue), clients_queues(clients_queues), world(b2Vec2(0,0), true) {

            clients_cars.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(0),
                std::forward_as_tuple(world, b2Vec2(5.f, 5.f))
            );
        }
