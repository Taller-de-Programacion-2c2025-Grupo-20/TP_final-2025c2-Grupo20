#include "queues_monitor.h"
#include <iostream>

Queue<GameStateDTO>& QueuesMonitor::addQueue(int client_id) {
    std::lock_guard<std::mutex> lock(mutex);
    return (queues.try_emplace(client_id).first->second).queue;
}

void QueuesMonitor::markQueueForDeletion(int client_id) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = queues.find(client_id);
    if (it != queues.end()) {
        it->second.is_alive = false;
        it->second.queue.close();
    }
}

void QueuesMonitor::broadcast(const GameStateDTO& res) {
    std::lock_guard<std::mutex> lock(mutex);

    for (auto& [client_id, entry] : queues) {
        if (!entry.is_alive) {
            continue;
        }

        try {
            entry.queue.try_push(res);
        } catch (const ClosedQueue&) {
            entry.is_alive = false;

        }
    }
}

QueuesMonitor::QueuesMonitor() {}

QueuesMonitor::~QueuesMonitor() {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& [id, entry] : queues) {
        try {
            entry.queue.close();
        } catch (const std::runtime_error& e) {
            std::cerr << "Ya estaba cerrada la queue del cliente " << id << ": " << e.what() << std::endl;
        }
    }
}
