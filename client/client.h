#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>
#include <string>
#include <atomic>
#include <SDL2/SDL.h>
#include "client_protocol.h"
#include "client_sender.h"
#include "client_receiver.h"
#include "../common/queue.h"
#include "../common/clientCommand.h"

class Client {
private:
    ClientProtocol protocol;
    //uint8_t my_player_id;
    std::atomic<bool> is_running;

    Queue<InputCmd> input_queue;
    ClientSender sender;
    ClientReceiver receiver;

    int run_game_loop();
    void handle_sdl_event(const SDL_Event& event);

public:
    Client(const char* host, const char* port);
    ~Client();

    void start_threads();
    void send_login_request(const std::string& username);
    void send_refresh_request();
    int runGame();

    ClientReceiver& getReceiver();
    uint8_t getMyPlayerId() const;
    ClientProtocol& getProtocol();
    void push_input(const InputCmd& cmd);
    
    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
};

#endif