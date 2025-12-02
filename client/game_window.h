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
#include <SDL2pp/Rect.hh> 
#include "client_sprites.h"
#include "client.h"

#include <SDL2pp/Mixer.hh>
#include "sound_manager.h"


#include <QApplication>
#include <QMainWindow> 

struct BoxMap {
    float s;
    int   ox, oy;
    SDL2pp::Rect panel;
};

class GameWindow {
private:
    Client &client;

    Queue<InputCmd> input_queue;
    Queue<GameStateDTO> state_queue;
    SpritesManager game_sprites;
    ClientReceiver& receiver;
    CarType car_to_use;
    int map_id;

    std::map<uint8_t, uint8_t> lastHealthByPlayer;

    std::map<uint8_t, float> lastSpeedByPlayer;

    float lastListenerX_m = 0.0f;
    float lastListenerY_m = 0.0f;

    bool running = false;
    bool backwards = false;
    int  turn_dir = 0;

    GameStateDTO finalState;
    bool raceFinished = false;
    bool braking = false;

    void syncFrame(double rate,
                           uint64_t perf_freq,
                           uint64_t& t1,
                           uint64_t& it);

    void drawMinimap(SDL2pp::Texture& background,
                     SDL2pp::Renderer& renderer,
                     GameStateDTO& last_state,
                     SDL2pp::Rect& dstRect,
                     SDL2pp::Texture& checkpoint_flag,
                     SDL2pp::Texture& checkered_flag);

    SDL2pp::Rect atlasToFit(const BoxMap& m, const SDL2pp::Rect& slotAtlas);

    BoxMap makeBoxMap(SDL2pp::Renderer& renderer,
                      SDL2pp::Texture& hud,
                      const SDL2pp::Rect& panelSrc,
                      int px, int py);
    int iround(float v);
    void drawDigitDst(SDL2pp::Renderer& renderer,
                      SDL2pp::Texture& hud,
                      int d,
                      const SDL2pp::Rect& dst);
    void drawCronometer(SDL2pp::Renderer& renderer,
                        SDL2pp::Texture& hud,
                        int hudX, int hudY,
                        GameStateDTO& last_state, const PlayerState* me);
    void drawSpeedometer(SDL2pp::Renderer& renderer,
                         SDL2pp::Texture& hud,
                         const PlayerState* me,
                         int hudX, int hudY);
    void drawHealthBar(SDL2pp::Renderer& renderer,
                       SDL2pp::Texture& hud,
                       int hudX, int hudY,
                       int hp);
    
    void drawGame(SDL2pp::Renderer& renderer,
                 SDL2pp::Texture& hud,
                 SDL2pp::Texture& background,
                 SDL2pp::Texture& sprites,
                 SDL2pp::Texture& checkpoint_flag,
                 SDL2pp::Texture& checkered_flag,
                 SDL2pp::Texture& checkpoint_hint,
                 SDL2pp::Texture& market,
                 SDL2pp::Texture& moriste,
                 SDL2pp::Rect& srcRect,
                 SDL2pp::Rect& dstRect,
                 int viewW, int viewH,
                 int bgW, int bgH,
                 double rate,
                 uint64_t perf_freq,
                 uint64_t& t1,
                 uint64_t& it,
                 GameStateDTO& last_state,
                 bool& have_state,
                 bool& exit,
                 int& my_player_index,
                 int& hp,int& actual_pos, float& pos_x_m, float& pos_y_m, float& angle, SoundManager& soundManager);

    void drawCheckpoint(SDL2pp::Renderer& renderer,
                        SDL2pp::Texture& checkpoint_flag,
                        SDL2pp::Texture& checkered_flag,
                        const GameStateDTO& state,
                        const SDL2pp::Rect& srcRect,
                        int viewW, int viewH);

    void drawCheckpointHintAroundCar(
    SDL2pp::Renderer& renderer,
    SDL2pp::Texture& checkpoint_hint,
    const GameStateDTO& state,
    const SDL2pp::Rect& srcRect,
    int viewW,
    int viewH);

    void drawMarket(
    SDL2pp::Renderer& renderer,
    SDL2pp::Texture& market,
    int viewW,
    int viewH,
    bool boughtSpeed,
    bool boughtAccel,
    bool boughtHealth);

    void drawMarketCountdown(SDL2pp::Renderer& renderer,
                            SDL2pp::Texture& market,
                            int seconds,
                            int viewW, int viewH);
        
    void drawUpgradesBar(SDL2pp::Renderer& renderer,
                        SDL2pp::Texture& hud,
                        int hudX, int hudY,
                        bool speedActive,
                        bool accelActive,
                        bool healthActive);

    void drawCheckpointCounter(SDL2pp::Renderer& renderer,
                            SDL2pp::Texture& hud,
                            int hudX, int hudY,
                            int count);

        
public:
    int runGame();
    uint8_t getCarType(int player_id);
    uint8_t getMapId();

    GameWindow(Client &client);

    GameWindow(const GameWindow&) = delete;
    GameWindow& operator=(const GameWindow&) = delete;

    ~GameWindow() = default;  
    
    GameStateDTO getFinalState() const { return finalState; }
    bool wasRaceFinished() const { return raceFinished; }
};

#endif
