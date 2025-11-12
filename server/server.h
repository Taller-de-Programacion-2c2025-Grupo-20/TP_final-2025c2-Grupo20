#ifndef SERVER_H
#define SERVER_H

#include <map>
#include <list>
#include <memory>
#include <mutex>
#include <atomic>
#include <string>

#include "../common/socket.h"
#include "../common/queue.h"
#include "../common/lobbyCommand.h"
#include "acceptor.h"
#include "client_handler.h"
#include "match.h"

class Server {
private:
    Socket listener; 
    Acceptor acceptor;
    std::atomic<bool> is_running;

    Queue<std::unique_ptr<ClientHandler>> new_clients_queue;    
    Queue<LobbyCommand> lobby_queue;

    std::mutex mtx; 
    std::list<std::unique_ptr<ClientHandler>> clients_in_lobby;
    std::map<uint8_t, std::unique_ptr<Match>> active_matches;
    uint8_t next_match_id;

    void process_new_clients();
    void process_lobby_commands();
    void reap_dead_lobby_clients();
    void cleanup_finished_matches();

    void handle_login(const LobbyCommand& cmd);
    void handle_create_match(const LobbyCommand& cmd);
    void handle_join_match(const LobbyCommand& cmd);
    void broadcast_lobby_state();

public:
    Server(const char* port);
    ~Server();
    
    int run();
    void stop();

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;
};

#endif 