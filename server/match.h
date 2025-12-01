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
    
    QueuesMonitor clients_queues;
    Queue<InputCmd> gameloop_queue;
    
    Gameloop gameloop; 
    
    std::list<std::unique_ptr<ClientHandler>> clients;
    std::mutex clients_mtx;

    uint8_t map_id;
    std::string map_name;
    std::map<uint8_t, CarType> player_cars;
    
    std::thread cleaner_thread;
    std::atomic<bool> keep_cleaning;
    void cleaner_loop();

    void reap_dead_clients();
    

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
    void set_map_id(uint8_t id);
    void set_player_car(uint8_t player_id, uint8_t car_id);
    uint8_t get_host_id();

    void run_match_logic(); 
    void broadcast_waiting_room_state();
};

#endif 