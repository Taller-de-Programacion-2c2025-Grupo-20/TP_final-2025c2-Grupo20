#include "server.h"

#include <iostream>

#include "../common/constants.h"
#include "../common/socket.h"

#include "acceptor.h"
#include "gameloop.h"

int Server::run() {

    Acceptor acceptor(port, gameloop_queue, clients_queues);
    Gameloop gameloop(gameloop_queue, clients_queues);

    acceptor.start();
    gameloop.start();

    while (std::cin.get() != 'q') {}

    gameloop.stop();
    acceptor.stop();

    acceptor.join();
    gameloop.join();

    return EXIT_SUCCESS;
}

Server::Server(const char* port): port(port), gameloop_queue(50) {}

Server::~Server() {}
