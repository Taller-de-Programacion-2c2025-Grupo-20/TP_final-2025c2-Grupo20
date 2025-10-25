#include "client.h"

#include <iostream>
#include <sstream>

#include "../common/constants.h"


int Client::run() {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream line_stream(line);
        std::string command;
        line_stream >> command;

        break;

    }

    return EXIT_SUCCESS;
}

Client::Client(const char* hostname, const char* port): protocol(hostname, port) {}

Client::~Client() {}
