#ifndef MATCH_H
#define MATCH_H

#include <list>
#include <memory>
#include <mutex>
#include <atomic>
#include <string>
#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/clientCommand.h"
#include "client_handler.h"
#include "gameloop.h"
#include "queues_monitor.h"

#define MAX_PLAYERS 8

class Match {
private:
    uint8_t match_id;
    const std::string name;
    std::atomic<bool> running;

    b2World world;
    
    QueuesMonitor clients_queues;
    Queue<InputCmd> gameloop_queue;
    
    Gameloop gameloop; 
    
    std::list<std::unique_ptr<ClientHandler>> clients;
    std::mutex clients_mtx;

    void reap_dead_clients();
    void broadcast_waiting_room_state();

public:
    Match(uint8_t id, const std::string& name);
    ~Match();

    void start();
    void stop();

    bool add_player(std::unique_ptr<ClientHandler> player);
    bool is_full() const;
    bool is_running() const;
    bool has_player(uint8_t player_id);
    uint8_t get_id() const;
    std::string get_name() const;
    size_t get_player_count();

    void run_match_logic(); 
};

#endif 