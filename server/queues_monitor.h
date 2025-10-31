#ifndef QUEUES_MONITOR_H
#define QUEUES_MONITOR_H

#include <mutex>
#include <unordered_map>

#include "../common/constants.h"
#include "../common/queue.h"
#include "../common/gameState.h"

struct SenderQueue {
    Queue<GameStateDTO> queue;
    bool is_alive;

    SenderQueue(): is_alive(true) {}
};

class QueuesMonitor {
private:
    std::unordered_map<int, SenderQueue> queues;
    std::mutex mutex;

public:
    Queue<GameStateDTO>& addQueue(int client_id);

    void markQueueForDeletion(int client_id);

    void broadcast(const GameStateDTO& res);

    QueuesMonitor();

    QueuesMonitor(const QueuesMonitor&) = delete;
    QueuesMonitor& operator=(const QueuesMonitor&) = delete;

    ~QueuesMonitor();
};

#endif
