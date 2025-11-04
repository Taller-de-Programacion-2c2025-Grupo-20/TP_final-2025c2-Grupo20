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
#include "client_sender.h"
#include "client_receiver.h"
#include "../common/gameState.h"
#include <SDL2pp/SDL.hh>
#include <SDL2pp/Window.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>

#include <QApplication>
#include <QMainWindow> 

class Client {
private:
    ClientProtocol protocol;
    int my_player_id;

    Queue<InputCmd> input_queue;
    Queue<GameStateDTO> state_queue;
    ClientSender sender;
    ClientReceiver receiver;

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
