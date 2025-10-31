#include "client_handler.h"

#include <string>

#include "../common/liberror.h"
#include "../common/socket.h"
#include "../common/gameState.h"

void ClientHandler::start() {
    receiver.start();
    sender.start();
}

void ClientHandler::stop() {
    receiver.stop();
    sender.stop();
}

void ClientHandler::join() {
    sender.join();
    receiver.join();
}

bool ClientHandler::is_alive() { return receiver.is_alive(); }

int ClientHandler::client_id() { return id; }

ClientHandler::ClientHandler(Socket&& skt, Queue<InputCmd>& gameloop_queue,
                             Queue<GameStateDTO>& client_queue, int id):
        protocol(std::move(skt)),
        id(id),
        receiver(protocol, gameloop_queue, id),
        sender(protocol, client_queue) {}

ClientHandler::~ClientHandler() {}
