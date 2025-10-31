#include "client.h"
#include "../common/gameState.h"
#include <iostream>


using namespace SDL2pp;

int Client::runClient() {
try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    // Create main window
    Window window("SDL2pp demo",
                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                  1600, 800,
                  SDL_WINDOW_RESIZABLE);

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    Surface surface(DATA_PATH "/cars/Mobile - Grand Theft Auto 4 - Miscellaneous - Cars.png");
    surface.SetColorKey(true, SDL_MapRGB(surface.Get()->format, 163, 163, 13));
    Texture sprites(renderer, surface);


    std::array<std::string, 3> maps = {
        "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - Liberty City.png",
        "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - San Andreas.png",
        "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - Vice City.png"
    };
    int map_to_play = 0;
    Texture background(renderer, DATA_PATH + maps[map_to_play]);


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

    //int car_to_use = 1;

    std::array<std::map<int, Rect>, 7> carsPositions = {
        carPositionsGreen, carPositionsRed, carPositionsDescapotable,
        carPositionsCeleste, carPositionsJeep, carPositionsCamioneta, carPositionsCamion
    };


    constexpr uint32_t TICK_MS = 16;
    uint64_t t1 = SDL_GetTicks64();


    Rect srcRect(0, 0, 1600, 800);
    Rect dstRect(0, 0, 1600, 800);

    //const int viewW = dstRect.GetW();
    //const int viewH = dstRect.GetH();

    while (true) {

        SDL_Event ev;
        while (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                InputCmd quit{};
                quit.key = InputKey::Quit;
                quit.action = InputAction::Press;
                input_queue.try_push(quit);
                return 0;
            }
            if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
                InputCmd cmd{};
                cmd.action = (ev.type == SDL_KEYDOWN) ? InputAction::Press : InputAction::Release;
                switch (ev.key.keysym.sym) {
                    case SDLK_UP:    cmd.key = InputKey::Up;    break;
                    case SDLK_DOWN:  cmd.key = InputKey::Down;  break;
                    case SDLK_LEFT:  cmd.key = InputKey::Left;  break;
                    case SDLK_RIGHT: cmd.key = InputKey::Right; break;
                    case SDLK_q:
                    case SDLK_ESCAPE: cmd.key = InputKey::Quit; break;
                }
                input_queue.try_push(cmd);
                if (cmd.key == InputKey::Quit && cmd.action == InputAction::Press) return 0;
            }
        }


        uint64_t now = SDL_GetTicks64();
        int64_t diff = static_cast<int64_t>(now - t1);
        int64_t ticks_to_run = diff / TICK_MS;
        for (int64_t i = 0; i < ticks_to_run; ++i) {
            t1 += TICK_MS;
        }

        GameStateDTO state;
        state_queue.try_pop(state);
        std::cout << state.players[0].state.x;
        std::cout << state.players[0].state.y;
        std::cout << state.players[0].state.angle;

        

        // if (
        //     GameStateDTO st = receiver.pollState()
        // ) {
            // pos_x = st->x;
            // pos_y = st->y;
            // actual_pos = st->sprite_idx;

            // int car_cx = static_cast<int>(std::lround(pos_x));
            // int car_cy = static_cast<int>(std::lround(pos_y));

            // int camX = car_cx - viewW / 2;
            // int camY = car_cy - viewH / 2;

            // const int bgW = background.GetWidth();
            // const int bgH = background.GetHeight();
            // const int maxCamX = std::max(0, bgW - viewW);
            // const int maxCamY = std::max(0, bgH - viewH);

            // camX = std::max(0, std::min(camX, maxCamX));
            // camY = std::max(0, std::min(camY, maxCamY));

            // srcRect.SetX(camX).SetY(camY);
        //}

        renderer.SetDrawColor(0, 0, 0, 255);
        renderer.Clear();

        renderer.Copy(background, srcRect, dstRect);

        // const Rect& spr = carsPositions[car_to_use][actual_pos];

        // int draw_x = static_cast<int>(std::lround(pos_x)) - spr.GetW() / 2 - srcRect.GetX();
        // int draw_y = static_cast<int>(std::lround(pos_y)) - spr.GetH() / 2 - srcRect.GetY();

        // renderer.Copy(sprites, spr, Rect(draw_x, draw_y, spr.GetW(), spr.GetH()));

        renderer.Present();

        // now  = SDL_GetTicks64();
        // diff = static_cast<int64_t>(t1 - now);
        // if (diff > 0) SDL_Delay(static_cast<uint32_t>(diff));
    }

    return 0;

} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}
}

Client::Client(const char* hostname, const char* port)
    : protocol(hostname, port),        
      my_player_id(0),                
      input_queue(128),
      state_queue(128),             
      sender(protocol, input_queue),  
      receiver(protocol, state_queue),            
      running(false),
      backwards(false),
      turn_dir(0)
{}

ClientProtocol& Client::getProtocol() {
    return protocol;
}    

bool Client::try_login(const std::string& username) {
    uint8_t player_id;
    
    protocol.send_login_attempt(username);

    uint8_t response_code = protocol.receive_login_response(player_id);

    if (response_code == CMD_LOGIN) {
        // Aca se podria guardar el player_id en un futuro
        return true;
    }
    
    return false; 
}


// int main(int argc, const char* argv[]) {
//     if (argc != 3) {
//         std::cerr << "Cantidad de parametros incorrecta\n";
//         return EXIT_FAILURE;
//     }
//     Client client(argv[1], argv[2]);
//     client.runClient();
//     return EXIT_SUCCESS;
// }
