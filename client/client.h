#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <exception>
#include <string>
#include <SDL2pp/SDL2pp.hh>
#include <cmath>
#include <map>
#include "client_protocol.h"

class Client {
private:
    ClientProtocol protocol;

public:
    int runClient();

    Client(const char* hostname, const char* port);

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    ~Client() = default;  
};

#endif
