#ifndef QUEUES_MONITOR_H
#define QUEUES_MONITOR_H

#include <mutex>
#include <unordered_map>

#include "../common_src/constants.h"
#include "../common_src/queue.h"

struct SenderQueue {
    Queue<ResponseDTO> queue;
    bool is_alive;

    SenderQueue(): is_alive(true) {}
};

class QueuesMonitor {
private:
    std::unordered_map<int, SenderQueue> queues;
    std::mutex mutex;

public:
    Queue<ResponseDTO>& addQueue(int client_id);

    void markQueueForDeletion(int client_id);

    void broadcast(const ResponseDTO& res);

    QueuesMonitor();

    QueuesMonitor(const QueuesMonitor&) = delete;
    QueuesMonitor& operator=(const QueuesMonitor&) = delete;

    ~QueuesMonitor();
};

#endif
