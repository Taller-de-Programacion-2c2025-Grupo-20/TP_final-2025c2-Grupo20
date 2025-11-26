#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <memory>
#include <atomic>
#include "../common/socket.h"
#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/clientCommand.h"
#include "../common/lobbyCommand.h"
#include "../common/gameState.h"
#include "../common/lobbyState.h"
#include "../common/match_list.h"
#include "server_protocol.h"
#include "sender.h"
#include "receiver.h"
#include "../common/liberror.h"

class ClientHandler {
private:
    uint8_t id;
    std::string username;
    ServerProtocol protocol;
    std::atomic<bool> is_alive_flag;
    uint8_t car_id;

    Receiver receiver;
    Sender sender;

public:
    ClientHandler(Socket&& skt, uint8_t id);
    ~ClientHandler();

    void stop();
    void join();
    bool is_alive() const;
    uint8_t get_id() const;
    void set_username(const std::string& name);
    std::string get_username() const;
    void set_car_id(uint8_t car_id);
    uint8_t get_car_id() const;
    
    void send_login_ok(uint8_t player_id);
    void send_login_failed();
    void start_in_lobby(Queue<LobbyCommand>& lobby_queue);
    void connect_to_match(Queue<InputCmd>& game_queue, Queue<GameStateDTO>& sender_queue);
    void send_lobby_update(const LobbyStateDTO& state);
    void send_match_list(const MatchListDTO& list);
    void send_start_game();
};

#endif