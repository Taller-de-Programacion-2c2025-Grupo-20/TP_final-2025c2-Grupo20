#include "queues_monitor.h"

Queue<ServerState>& QueuesMonitor::addQueue(int client_id) {
    std::lock_guard<std::mutex> lock(mutex);
    return (queues.try_emplace(client_id).first->second).queue;
}

void QueuesMonitor::markQueueForDeletion(int client_id) {
    std::lock_guard<std::mutex> lock(mutex);
    queues[client_id].is_alive = false;
}

void QueuesMonitor::broadcast(const ServerState& res) {
    std::lock_guard<std::mutex> lock(mutex);

    for (auto it = queues.begin(); it != queues.end();) {
        if (it->second.is_alive) {
            it->second.queue.try_push(res);
            ++it;
        } else {
            it = queues.erase(it);
        }
    }
}

QueuesMonitor::QueuesMonitor() {}

QueuesMonitor::~QueuesMonitor() {}
