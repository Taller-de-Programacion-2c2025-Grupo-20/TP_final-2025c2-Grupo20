#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H
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
#include "client.h"
#include "../common/gameState.h"
#include <SDL2pp/SDL.hh>
#include <SDL2pp/Window.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Surface.hh>
#include <SDL2pp/Texture.hh>

#include <QApplication>
#include <QMainWindow> 

class GameWindow {
private:

    Client &client;
    ClientReceiver& receiver;

    bool running = false;
    bool backwards = false;
    int  turn_dir = 0;


public:
    int runGame();

    GameWindow(Client &client);

    GameWindow(const GameWindow&) = delete;
    GameWindow& operator=(const GameWindow&) = delete;

    ~GameWindow() = default;  
    
};

#endif
