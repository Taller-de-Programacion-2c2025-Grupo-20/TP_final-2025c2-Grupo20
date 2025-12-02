#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <QObject>
#include <atomic>
#include <mutex>

#include "../common/gameState.h"
#include "../common/lobbyState.h"
#include "../common/match_list.h"
#include "../common/queue.h"
#include "../common/thread.h"

#include "client_protocol.h"

class ClientReceiver: public QObject, public Thread {
    Q_OBJECT

private:
    ClientProtocol& protocol;
    std::atomic<bool> is_authenticated;
    uint8_t my_player_id;

    Queue<LobbyStateDTO> lobby_state_queue;
    Queue<GameStateDTO> game_state_queue;
    Queue<MatchListDTO> match_list_queue;
    MatchListDTO last_match_list;

    GameStateDTO last_game_state;
    LobbyStateDTO last_lobby_state;
    std::mutex mtx;
    std::atomic<bool> server_down{false};
    void run() override;

public:
    ClientReceiver(ClientProtocol& protocol, QObject* parent = nullptr);

    GameStateDTO pollGameState();
    LobbyStateDTO pollLobbyState();
    MatchListDTO pollMatchList();

    bool is_logged_in() const;
    uint8_t get_my_id() const;
    void stop() override;
    uint8_t get_car_type(int player_id);
    uint8_t get_map_id();
    bool isServerDown() const;

signals:
    void loginSuccess(uint8_t player_id);
    void loginFailed();
    void gameStarted();
};

#endif