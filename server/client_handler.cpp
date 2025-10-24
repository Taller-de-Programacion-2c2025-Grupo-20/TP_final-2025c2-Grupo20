#include "client_handler.h"

#include <string>

#include "../common_src/liberror.h"
#include "../common_src/socket.h"

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

ClientHandler::ClientHandler(Socket&& skt, Queue<Instruction>& gameloop_queue,
                             Queue<ResponseDTO>& client_queue, int id):
        protocol(std::move(skt)),
        id(id),
        receiver(protocol, gameloop_queue, id),
        sender(protocol, client_queue) {}

ClientHandler::~ClientHandler() {}
