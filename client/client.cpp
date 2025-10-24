#include "client.h"

#include <iostream>
#include <sstream>

#include "../common_src/constants.h"

std::string Client::receiveMessage() {
    ResponseDTO message = protocol.receiveMessage();
    if (message.command == SEND_CARS_WITH_NITRO) {

        if (message.nitro_state == NITRO_ACTIVATED) {
            return "A car hit the nitro!";
        }

        if (message.nitro_state == NITRO_DEACTIVATED) {
            return "A car is out of juice.";
        }
    }

    return "Unknown command";
}

void Client::receiveMessages(int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << receiveMessage() << "\n";
    }
}

int Client::run() {
    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream line_stream(line);
        std::string command;
        line_stream >> command;

        if (command == "exit") {
            break;
        }

        if (command == "nitro") {
            protocol.sendNitro();
        }

        if (command == "read") {
            int arg = 0;
            if (line_stream >> arg) {
                receiveMessages(arg);
            }
        }
    }

    return EXIT_SUCCESS;
}

Client::Client(const char* hostname, const char* port): protocol(hostname, port) {}

Client::~Client() {}
