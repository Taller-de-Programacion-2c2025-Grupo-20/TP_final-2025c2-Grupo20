#ifndef GAMELOOP_H
#define GAMELOOP_H

#include <unordered_map>

#include "../common_src/queue.h"
#include "../common_src/thread.h"

#include "queues_monitor.h"

class Gameloop: public Thread {
private:
    Queue<Instruction>& gameloop_queue;
    QueuesMonitor& clients_queues;

    std::unordered_map<int, int> clients_with_nitro;

    void processInstruction(const Instruction& ins);

    void updateClientsNitroState();

public:
    void run() override;

    void stop() override;

    Gameloop(Queue<Instruction>& gameloop_queue, QueuesMonitor& clients_queues);
};


#endif
