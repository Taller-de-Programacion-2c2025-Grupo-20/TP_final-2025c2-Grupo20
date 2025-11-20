#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/gameState.h"
#include "../common/lobbyState.h"
#include "../common/match_list.h"
#include "client_protocol.h"
#include <atomic>
#include <mutex>
#include <QObject>

class ClientReceiver : public QObject, public Thread {
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

    void run() override;

public:
    ClientReceiver(ClientProtocol& protocol, QObject* parent = nullptr);
    
    GameStateDTO pollGameState();
    LobbyStateDTO pollLobbyState();
    MatchListDTO pollMatchList();

    bool is_logged_in() const;
    uint8_t get_my_id() const;
    void stop() override;

signals:
    void loginSuccess(uint8_t player_id);
    void loginFailed();
    void gameStarted();
};

#endif