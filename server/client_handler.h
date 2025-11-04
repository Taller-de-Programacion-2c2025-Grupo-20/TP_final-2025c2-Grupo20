#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <utility>

#include "../common/queue.h"
#include "../common/clientCommand.h"
#include "../common/gameState.h"

#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"

class ClientHandler {
private:
    ServerProtocol protocol;
    uint8_t id;

    Receiver receiver;
    Sender sender;

public:
    void start();
    void stop();
    void join();
    bool is_alive();

    uint8_t client_id();

    ClientHandler(Socket&& skt, Queue<InputCmd>& gameloop_queue,
                  Queue<GameStateDTO>& client_queue, uint8_t id);

    ClientHandler(const ClientHandler&) = delete;
    ClientHandler& operator=(const ClientHandler&) = delete;

    ~ClientHandler();
};

#endif
