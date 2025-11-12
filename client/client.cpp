#include "client.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <QObject>

Client::Client(const char* hostname, const char* port) : 
    protocol(hostname, port),
    my_player_id(0),
    is_running(true),
    input_queue(128),
    sender(protocol, input_queue),
    receiver(protocol)
{}

Client::~Client() {
    is_running = false;
    sender.stop();
    receiver.stop();
    sender.join();
    receiver.join();
}

void Client::start_threads() {
    receiver.start();
    sender.start();
}

void Client::send_login_request(const std::string& username) {
    protocol.send_login_attempt(username);
}

int Client::runGame() {
    return run_game_loop();
}

uint8_t Client::getMyPlayerId() const {
    return my_player_id;
}

ClientReceiver& Client::getReceiver() {
    return receiver;
}

ClientProtocol& Client::getProtocol() { return protocol; }

void Client::push_input(const InputCmd& cmd) {
    input_queue.try_push(cmd);
}

int Client::run_game_loop() {
    std::cout << "¡El juego ha comenzado!" << std::endl;
    
    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_running = false;
                break;
            }
            handle_sdl_event(event);
        }

        GameStateDTO game_state = receiver.pollGameState();

        std::cout << "Game state update. Mi auto está en: " 
                  << game_state.players[0].state.x 
                  << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}

void Client::handle_sdl_event(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        InputCmd cmd{};
        cmd.player_id = my_player_id;
        cmd.action = (event.type == SDL_KEYDOWN) ? InputAction::Press : InputAction::Release;
        
        switch (event.key.keysym.sym) {
            case SDLK_UP:    cmd.key = InputKey::Up;    break;
            case SDLK_DOWN:  cmd.key = InputKey::Down;  break;
            case SDLK_LEFT:  cmd.key = InputKey::Left;  break;
            case SDLK_RIGHT: cmd.key = InputKey::Right; break;
            case SDLK_q:
            case SDLK_ESCAPE: cmd.key = InputKey::Quit; break;
            default:         cmd.key = InputKey::Unknown; break;
        }

        if (cmd.key == InputKey::Quit && cmd.action == InputAction::Press) {
            is_running = false;
            return;
        }
        
        if (cmd.key != InputKey::Unknown) {
            push_input(cmd);
        }
    }
}