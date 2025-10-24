#include "gameloop.h"

#include <chrono>
#include <thread>

#include "../common_src/constants.h"
#include "../common_src/queue.h"

void Gameloop::processInstruction(const Instruction& ins) {
    if (ins.command != SEND_NITRO) {
        return;
    }

    auto [it, was_inserted] = clients_with_nitro.try_emplace(ins.client_id, 0);
    if (!was_inserted) {
        return;
    }

    std::cout << "A car hit the nitro!\n";
    clients_queues.broadcast(
            ResponseDTO(SEND_CARS_WITH_NITRO, clients_with_nitro.size(), NITRO_ACTIVATED));
}

void Gameloop::updateClientsNitroState() {
    for (auto it = clients_with_nitro.begin(); it != clients_with_nitro.end();) {
        if (it->second == 12) {
            std::cout << "A car is out of juice.\n";
            it = clients_with_nitro.erase(it);
            clients_queues.broadcast(ResponseDTO(SEND_CARS_WITH_NITRO, clients_with_nitro.size(),
                                                 NITRO_DEACTIVATED));
        } else {
            it->second++;
            ++it;
        }
    }
}

void Gameloop::run() {

    while (should_keep_running()) {
        try {
            Instruction ins;
            while (gameloop_queue.try_pop(ins)) {
                processInstruction(ins);
            }

            updateClientsNitroState();

            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        } catch (const ClosedQueue&) {
            break;
        }
    }
}

void Gameloop::stop() {
    Thread::stop();
    gameloop_queue.close();
}

Gameloop::Gameloop(Queue<Instruction>& gameloop_queue, QueuesMonitor& clients_queues):
        gameloop_queue(gameloop_queue), clients_queues(clients_queues) {}
