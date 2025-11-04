#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>
#include <string>
#include "client_protocol.h"

class Client {
private:
    ClientProtocol protocol;
    uint8_t my_player_id;

public:
    Client(const char* host, const char* port);

    bool try_login(const std::string& username);
    int runGame();

    ClientProtocol& getProtocol() { return protocol; }
    uint8_t getMyPlayerId() const { return my_player_id; }

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
    ~Client() = default;
};

#endif
