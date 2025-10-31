#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <exception>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  

#include <cmath>
#include <map>
#include "client_protocol.h"
#include <algorithm>
#include "../common/clientCommand.h"
#include "../common/queue.h"
#include "sender.h"
#include "receiver.h"

class Client {
private:
    ClientProtocol protocol;
    uint8_t my_player_id;

    Queue<InputCmd> sender_queue;
    Sender sender;
    Receiver receiver;

    bool running = false;
    bool backwards = false;
    int  turn_dir = 0;
    
    void handle_input(const InputCmd& cmd);

public:
    int runClient();

    Client(const char* hostname, const char* port);

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    ~Client() = default;  

    ClientProtocol& getProtocol();
    bool try_login(const std::string& username);
};

#endif
