#ifndef CLIENT_H
#define CLIENT_H

#include <map>
#include <string>
#include <vector>

#include "../common/socket.h"

#include "client_protocol.h"

class Client {
private:
    ClientProtocol protocol;

public:
    int run();

    Client(const char* hostname, const char* port);

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    ~Client();
};

#endif
