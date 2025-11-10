#include "client.h"
#include "game_window.h"
#include "../common/gameState.h"
#include <iostream>
#include <array>
#include <map>
#include <algorithm>
#include <cmath>

#ifndef DATA_PATH
#define DATA_PATH "../data"
#endif

using namespace SDL2pp;

static constexpr float PPM = 16.0f;
static constexpr float PI  = 3.14159265358979323846f;


static inline int angle_to_frame(float rad) {
    float a = std::fmod(rad, 2.0f * PI);
    if (a < 0) a += 2.0f * PI;

    int idx = (int)std::lround((a) * 16.0f / (2.0f * PI)) & 15;

    return (4 + idx) & 15;
}


int GameWindow::runGame() {
    try {
        receiver.start();
        sender.start();

        // Init SDL
        SDL sdl(SDL_INIT_VIDEO);

        // Hints ANTES de crear Window/Renderer
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
        SDL_SetHint(SDL_HINT_RENDER_DRIVER, "vulkan");

        // Ventana y renderer
        Window window("SDL2pp demo",
                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                      1600, 800,
                      SDL_WINDOW_RESIZABLE);

        Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

        Surface surface(DATA_PATH "/cars/Mobile - Grand Theft Auto 4 - Miscellaneous - Cars.png");
        surface.SetColorKey(true, SDL_MapRGB(surface.Get()->format, 163, 163, 13));
        Texture sprites(renderer, surface);

        Surface life_bar_surface(DATA_PATH "/assets/life_bar.png");
        Texture healthsprite(renderer, life_bar_surface);

        std::array<std::string, 3> maps = {
            "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - Liberty City.png",
            "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - San Andreas.png",
            "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - Vice City.png"
        };
        int map_to_play = 0;
        Texture background(renderer, DATA_PATH + maps[map_to_play]);
        const int bgW = background.GetWidth();
        const int bgH = background.GetHeight();


        std::map<int, Rect> healthBarStates = {
            {5, Rect(44, 56, 225, 55)},
            {4, Rect(44, 189, 225, 55)},
            {3, Rect(44, 324, 225, 55)},
            {2, Rect(308, 56, 225, 55)},
            {1, Rect(308, 189, 225, 55)},
            {0, Rect(308, 324, 225, 55)}
        };

        std::map<int, Rect> carPositionsGreen = {
            {0, Rect(0, 0, 32, 32)}, {1, Rect(32, 0, 32, 32)}, {2, Rect(64, 0, 32, 32)}, {3, Rect(96, 0, 32, 32)},
            {4, Rect(128, 0, 32, 32)}, {5, Rect(160, 0, 32, 32)}, {6, Rect(192, 0, 32, 32)}, {7, Rect(224, 0, 32, 32)},
            {8, Rect(0, 32, 32, 32)}, {9, Rect(32, 32, 32, 32)}, {10, Rect(64, 32, 32, 32)}, {11, Rect(96, 32, 32, 32)},
            {12, Rect(128, 32, 32, 32)}, {13, Rect(160, 32, 32, 32)}, {14, Rect(192, 32, 32, 32)}, {15, Rect(224, 32, 32, 32)}
        };
        std::map<int, Rect> carPositionsRed = {
            {0, Rect(0, 64, 40, 40)}, {1, Rect(40, 64, 40, 40)}, {2, Rect(80, 64, 40, 40)}, {3, Rect(120, 64, 40, 40)},
            {4, Rect(160, 64, 40, 40)}, {5, Rect(200, 64, 40, 40)}, {6, Rect(240, 64, 40, 40)}, {7, Rect(280, 64, 40, 40)},
            {8, Rect(0, 104, 40, 40)}, {9, Rect(40, 104, 40, 40)}, {10, Rect(80, 104, 40, 40)}, {11, Rect(120, 104, 40, 40)},
            {12, Rect(160, 104, 40, 40)}, {13, Rect(200, 104, 40, 40)}, {14, Rect(240, 104, 40, 40)}, {15, Rect(280, 104, 40, 40)}
        };
        std::map<int, Rect> carPositionsDescapotable = {
            {0, Rect(0, 144, 40, 40)}, {1, Rect(40, 144, 40, 40)}, {2, Rect(80, 144, 40, 40)}, {3, Rect(120, 144, 40, 40)},
            {4, Rect(160, 144, 40, 40)}, {5, Rect(200, 144, 40, 40)}, {6, Rect(240, 144, 40, 40)}, {7, Rect(280, 144, 40, 40)},
            {8, Rect(0, 184, 40, 40)}, {9, Rect(40, 184, 40, 40)}, {10, Rect(80, 184, 40, 40)}, {11, Rect(120, 184, 40, 40)},
            {12, Rect(160, 184, 40, 40)}, {13, Rect(200, 184, 40, 40)}, {14, Rect(240, 184, 40, 40)}, {15, Rect(280, 184, 40, 40)}
        };
        std::map<int, Rect> carPositionsCeleste = {
            {0, Rect(0, 224, 40, 40)}, {1, Rect(40, 224, 40, 40)}, {2, Rect(80, 224, 40, 40)}, {3, Rect(120, 224, 40, 40)},
            {4, Rect(160, 224, 40, 40)}, {5, Rect(200, 224, 40, 40)}, {6, Rect(240, 224, 40, 40)}, {7, Rect(280, 224, 40, 40)},
            {8, Rect(0, 264, 40, 40)}, {9, Rect(40, 264, 40, 40)}, {10, Rect(80, 264, 40, 40)}, {11, Rect(120, 264, 40, 40)},
            {12, Rect(160, 264, 40, 40)}, {13, Rect(200, 264, 40, 40)}, {14, Rect(240, 264, 40, 40)}, {15, Rect(280, 264, 40, 40)}
        };
        std::map<int, Rect> carPositionsJeep = {
            {0, Rect(0, 304, 40, 40)}, {1, Rect(40, 304, 40, 40)}, {2, Rect(80, 304, 40, 40)}, {3, Rect(120, 304, 40, 40)},
            {4, Rect(160, 304, 40, 40)}, {5, Rect(200, 304, 40, 40)}, {6, Rect(240, 304, 40, 40)}, {7, Rect(280, 304, 40, 40)},
            {8, Rect(0, 344, 40, 40)}, {9, Rect(40, 344, 40, 40)}, {10, Rect(80, 344, 40, 40)}, {11, Rect(120, 344, 40, 40)},
            {12, Rect(160, 344, 40, 40)}, {13, Rect(200, 344, 40, 40)}, {14, Rect(240, 344, 40, 40)}, {15, Rect(280, 344, 40, 40)}
        };
        std::map<int, Rect> carPositionsCamioneta = {
            {0, Rect(0, 384, 40, 40)}, {1, Rect(40, 384, 40, 40)}, {2, Rect(80, 384, 40, 40)}, {3, Rect(120, 384, 40, 40)},
            {4, Rect(160, 384, 40, 40)}, {5, Rect(200, 384, 40, 40)}, {6, Rect(240, 384, 40, 40)}, {7, Rect(280, 384, 40, 40)},
            {8, Rect(0, 424, 40, 40)}, {9, Rect(40, 424, 40, 40)}, {10, Rect(80, 424, 40, 40)}, {11, Rect(120, 424, 40, 40)},
            {12, Rect(160, 424, 40, 40)}, {13, Rect(200, 424, 40, 40)}, {14, Rect(240, 424, 40, 40)}, {15, Rect(280, 424, 40, 40)}
        };
        std::map<int, Rect> carPositionsCamion = {
            {0, Rect(0, 464, 48, 48)}, {1, Rect(48, 464, 48, 48)}, {2, Rect(96, 464, 48, 48)}, {3, Rect(144, 464, 48, 48)},
            {4, Rect(192, 464, 48, 48)}, {5, Rect(240, 464, 48, 48)}, {6, Rect(288, 464, 48, 48)}, {7, Rect(336, 464, 48, 48)},
            {8, Rect(0, 512, 48, 48)}, {9, Rect(48, 512, 48, 48)}, {10, Rect(96, 512, 48, 48)}, {11, Rect(144, 512, 48, 48)},
            {12, Rect(192, 512, 48, 48)}, {13, Rect(240, 512, 48, 48)}, {14, Rect(288, 512, 48, 48)}, {15, Rect(336, 512, 48, 48)}
        };

        int car_to_use = 2;
        std::array<std::map<int, Rect>, 7> carsPositions = {
            carPositionsGreen, carPositionsRed, carPositionsDescapotable,
            carPositionsCeleste, carPositionsJeep, carPositionsCamioneta, carPositionsCamion
        };

        const double rate = 1.0 / 60.0;
        const uint64_t perf_freq = SDL_GetPerformanceFrequency();
        uint64_t t1 = SDL_GetPerformanceCounter();
        uint64_t it = 0;

        Rect srcRect(0, 0, 1600, 800);
        Rect dstRect(0, 0, 1600, 800);
        const int viewW = dstRect.GetW();
        const int viewH = dstRect.GetH();

        GameStateDTO last_state;
        bool have_state = false;
        bool exit = false;
        int   actual_pos = 0;
        float pos_x_m = 0.f, pos_y_m = 0.f, angle = 0.f;
        int health_amount_current = 0;

        while (true) {

            SDL_Event ev;
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_QUIT) {
                    InputCmd quit{};
                    quit.player_id = client.getMyPlayerId();
                    quit.key = InputKey::Quit;
                    quit.action = InputAction::Press;
                    input_queue.try_push(quit);
                    exit = true;
                    break;
                }
                if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
                    InputCmd cmd{};
                    cmd.player_id = client.getMyPlayerId();
                    cmd.action = (ev.type == SDL_KEYDOWN) ? InputAction::Press : InputAction::Release;
                    switch (ev.key.keysym.sym) {
                        case SDLK_UP:    cmd.key = InputKey::Up; input_queue.try_push(cmd);break;
                        case SDLK_w:    cmd.key = InputKey::Up; input_queue.try_push(cmd); break;
                        case SDLK_DOWN:  cmd.key = InputKey::Down; input_queue.try_push(cmd); break;
                        case SDLK_s:  cmd.key = InputKey::Down; input_queue.try_push(cmd); break;
                        case SDLK_LEFT:  cmd.key = InputKey::Left; input_queue.try_push(cmd); break;
                        case SDLK_a:  cmd.key = InputKey::Left; input_queue.try_push(cmd); break;
                        case SDLK_RIGHT: cmd.key = InputKey::Right; input_queue.try_push(cmd); break;
                        case SDLK_d: cmd.key = InputKey::Right; input_queue.try_push(cmd); break;
                        case SDLK_q:
                        case SDLK_ESCAPE: cmd.key = InputKey::Quit; input_queue.try_push(cmd); break;
                    }
                    
                    if (cmd.key == InputKey::Quit && cmd.action == InputAction::Press) {
                        exit = true;
                        break;
                    }
                }
            }

            if (exit){
                break;
            }

            GameStateDTO gs;
            bool got = false;
            while (state_queue.try_pop(gs)) {
                last_state = std::move(gs);
                got = true;
            }
            
            if (got == true or have_state == true){
                have_state = 1;
            }
            std::cout << "Mi player id es: " << static_cast<int>(client.getMyPlayerId()) << "\n";


            renderer.SetDrawColor(0, 0, 0, 255);
            renderer.Clear();

            renderer.Copy(background, srcRect, dstRect);

            for (size_t i = 0; i < last_state.players.size(); i++) {

                if (have_state && last_state.players[i].player_id == client.getMyPlayerId()) {
                    const auto& st = last_state.players[i].state;
                    pos_x_m = st.x;
                    pos_y_m = st.y;
                    angle = st.angle;
                    actual_pos = angle_to_frame(angle);

                    int health_amount = last_state.players[i].health / 20;
                    if (health_amount > 5) health_amount = 5;
                    health_amount_current = health_amount;
                    
                    std::cout << "Vida restante recibida: " << static_cast<int>(last_state.players[i].health) << "\n";
                    std::cout << "Vida restante final: " << static_cast<int>(health_amount_current) << "\n";

                    const int car_cx_px = static_cast<int>(std::lround(pos_x_m * PPM));
                    const int car_cy_px = static_cast<int>(std::lround(pos_y_m * PPM));

                    int camX = car_cx_px - viewW / 2;
                    int camY = car_cy_px - viewH / 2;

                    camX = std::clamp(camX, 0, std::max(0, bgW - viewW));
                    camY = std::clamp(camY, 0, std::max(0, bgH - viewH));

                    srcRect.SetX(camX).SetY(camY);
                }

                const auto& st = last_state.players[i].state;
                pos_x_m = st.x;
                pos_y_m = st.y;
                angle = st.angle;
                actual_pos = angle_to_frame(angle);

                const Rect& spr = carsPositions[car_to_use][actual_pos];
                const int car_x_px = static_cast<int>(pos_x_m * PPM + 0.5f);
                const int car_y_px = static_cast<int>(pos_y_m * PPM + 0.5f);
                
                const int draw_x = car_x_px - spr.GetW() / 2 - srcRect.GetX();
                const int draw_y = car_y_px - spr.GetH() / 2 - srcRect.GetY();

                if (draw_x + spr.GetW() < 0 || draw_x > viewW ||
                    draw_y + spr.GetH() < 0 || draw_y > viewH) {
                    continue;
                }

                renderer.Copy(sprites, spr, Rect(draw_x, draw_y, spr.GetW(), spr.GetH()));
            }
            const Rect& health_src = healthBarStates.at(health_amount_current);
            renderer.Copy(healthsprite, health_src, Rect(5, 5, 0.65*(health_src.GetW()), 0.65*(health_src.GetH())));

            renderer.Present();
            

            uint64_t t2 = SDL_GetPerformanceCounter();
            double elapsed = static_cast<double>(t2 - t1) / perf_freq;
            double rest = rate - elapsed;

            if (rest > 0.0) {

                double rest_ms = rest * 1000.0;
                if (rest_ms > 1.5) {
                    SDL_Delay(static_cast<Uint32>(rest_ms - 1.0));
                }

                for (;;) {
                    uint64_t now = SDL_GetPerformanceCounter();
                    if ((double)(now - t1) / perf_freq >= rate) break;
                }
            } else {

                double behind = -rest;
                double lost = behind - std::fmod(behind, rate);
                t1 += static_cast<uint64_t>(lost * perf_freq);
                it += static_cast<uint64_t>(lost / rate);
            }


            t1 += static_cast<uint64_t>(rate * perf_freq);
            ++it;
        }


        receiver.stop();
        sender.stop();
        receiver.join();
        sender.join();
        return 0;

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

GameWindow::GameWindow(Client& c)
    : client(c),
      sender(c.getProtocol(), input_queue),
      receiver(c.getProtocol(), state_queue) {}
