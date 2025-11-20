#ifndef RECEIVER_H
#define RECEIVER_H

#include <mutex>
#include "../common/queue.h"
#include "../common/thread.h"
#include "../common/clientCommand.h"
#include "../common/lobbyCommand.h"
#include "server_protocol.h"

class Receiver: public Thread {
private:
    ServerProtocol& protocol;
    uint8_t id;

    Queue<InputCmd>* gameloop_queue;
    Queue<LobbyCommand>* lobby_queue;
    std::mutex mtx;

public:
    void run() override;
    void stop() override;

    Receiver(ServerProtocol& protocol, uint8_t id);

    void set_lobby_queue(Queue<LobbyCommand>& new_queue);
    void set_game_queue(Queue<InputCmd>& new_queue);
};

#endif