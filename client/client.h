#ifndef CLIENT_H
#define CLIENT_H

#include <atomic>
#include <cstdint>
#include <string>

#include <SDL2/SDL.h>

#include "../common/clientCommand.h"
#include "../common/queue.h"

#include "client_protocol.h"
#include "client_receiver.h"
#include "client_sender.h"

class Client {
private:
    ClientProtocol protocol;
    // uint8_t my_player_id;
    std::atomic<bool> is_running;
    bool finished_game = false;
    std::vector<PlayerResultDTO> final_results;
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
    uint8_t getCarType(int player_id);
    uint8_t getMapId();
    bool hasFinishedGame() { return finished_game; }
    void setFinishedGame() { finished_game = true; }
    void push_input(const InputCmd& cmd);
    void setFinalResults(std::vector<PlayerResultDTO> results) { final_results = results; }
    std::vector<PlayerResultDTO> getFinalState() { return final_results; }

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;
};

#endif