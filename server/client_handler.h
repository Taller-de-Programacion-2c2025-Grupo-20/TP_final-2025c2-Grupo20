#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <utility>

#include "../common_src/queue.h"

#include "receiver.h"
#include "sender.h"
#include "server_protocol.h"

class ClientHandler {
private:
    ServerProtocol protocol;
    int id;

    Receiver receiver;
    Sender sender;

public:
    void start();
    void stop();
    void join();
    bool is_alive();

    int client_id();

    ClientHandler(Socket&& skt, Queue<Instruction>& gameloop_queue,
                  Queue<ResponseDTO>& client_queue, int id);

    ClientHandler(const ClientHandler&) = delete;
    ClientHandler& operator=(const ClientHandler&) = delete;

    ~ClientHandler();
};

#endif
