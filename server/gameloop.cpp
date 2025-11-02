#include "gameloop.h"

#include <chrono>
#include <thread>

#include "car.h"
#include "../common/constants.h"
#include "../common/queue.h"

const float timeStep = 1.f/60.f;

void Gameloop::run() {

    Car test_car(world, b2Vec2(5.f, 5.f));

    while (should_keep_running()) {
        try {
            InputCmd input;
            while (gameloop_queue.try_pop(input)) {
                test_car.handleInput(input);
                std::cout << "Pos del auto en x:" << test_car.position().x << "Pos del auto en y:" << test_car.position().y << "\n" << "Angulo:" << test_car.angle() << "\n";
            }
            
            test_car.updateCarPhysics();
            world.Step(timeStep, 6, 2);

            GameStateDTO current_state;

            PlayerState test_player_state;
            test_player_state.player_id = 0; 
            test_player_state.state = ServerState(test_car.position().x, test_car.position().y, test_car.angle());
            test_player_state.health = 100;

            current_state.players.push_back(test_player_state);
            current_state.car_count = current_state.players.size();

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
        gameloop_queue(gameloop_queue), clients_queues(clients_queues), world(b2Vec2(0,0), true) {}
