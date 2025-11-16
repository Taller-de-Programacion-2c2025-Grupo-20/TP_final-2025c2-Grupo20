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
static constexpr int BOX_W = 240;
static constexpr int BOX_H = 60;


static inline int angle_to_frame(float rad) {
    float a = std::fmod(rad, 2.0f * PI);
    if (a < 0) a += 2.0f * PI;

    int idx = (int)std::lround((a) * 16.0f / (2.0f * PI)) & 15;

    return (4 + idx) & 15;
}

int GameWindow::iround(float v) {
    return static_cast<int>(std::lround(v));
}


void GameWindow::drawDigitDst(Renderer& renderer, Texture& hud, int d, const Rect& dst) {
    const Rect& src = game_sprites.getDigitRect(d);
    renderer.Copy(hud, src, dst);
}

BoxMap GameWindow::makeBoxMap(Renderer& renderer, Texture& hud,
                              const Rect& panelSrc, int px, int py){
    float aspect_src = (float)panelSrc.GetW() / (float)panelSrc.GetH();
    float aspect_box = (float)BOX_W / (float)BOX_H;

    float scale;
    if (aspect_src > aspect_box)
        scale = (float)BOX_W / (float)panelSrc.GetW();
    else
        scale = (float)BOX_H / (float)panelSrc.GetH();

    int w = iround(panelSrc.GetW() * scale);
    int h = iround(panelSrc.GetH() * scale);

    int ox = px + (BOX_W - w) / 2;
    int oy = py + (BOX_H - h) / 2;

    renderer.Copy(hud, panelSrc, Rect(ox, oy, w, h));

    return {scale, ox, oy, panelSrc};
}

Rect GameWindow::atlasToFit(const BoxMap& m, const Rect& slotAtlas){
    int dx = slotAtlas.GetX() - m.panel.GetX();
    int dy = slotAtlas.GetY() - m.panel.GetY();
    return Rect(
        m.ox + iround(dx * m.s),
        m.oy + iround(dy * m.s),
        iround(slotAtlas.GetW() * m.s),
        iround(slotAtlas.GetH() * m.s)
    );
}
void GameWindow::drawHealthBar(Renderer& renderer, Texture& hud,
                               int hudX, int hudY, int hp) {
    const Rect& PANEL_HP = game_sprites.getHealthPanelRect();
    BoxMap hpMap = makeBoxMap(renderer, hud, PANEL_HP, hudX, hudY);

    int hp_clamped = std::clamp(hp, 0, 999);

    drawDigitDst(renderer, hud, (hp_clamped/100)%10,
                 atlasToFit(hpMap, game_sprites.getHealthDigitRectHundreds()));
    drawDigitDst(renderer, hud, (hp_clamped/10)%10,
                 atlasToFit(hpMap, game_sprites.getHealthDigitRectTens()));
    drawDigitDst(renderer, hud, hp_clamped%10,
                 atlasToFit(hpMap, game_sprites.getHealthDigitRectUnits()));

    int hearts = std::clamp(hp/20, 0, 5);
    for (int k = 0; k < 5; ++k) {
        const Rect& heartSrc = (k < hearts)
            ? game_sprites.getFullHeartRect()
            : game_sprites.getEmptyHeartRect();

        renderer.Copy(hud, heartSrc,
                      atlasToFit(hpMap, game_sprites.getHeartSlotRect(k)));
    }
}

void GameWindow::drawCronometer(Renderer& renderer, Texture& hud,
                                int hudX, int hudY, GameStateDTO& last_state)
{
    const Rect& PANEL_TIME  = game_sprites.getTimePanelRect();
    BoxMap timeMap = makeBoxMap(renderer, hud, PANEL_TIME, hudX, hudY);

    int total = static_cast<int>(last_state.elapsed_time);
    int mm = (total / 60) % 100;
    int ss = total % 60;

    drawDigitDst(renderer, hud, (mm/10)%10, atlasToFit(timeMap, game_sprites.getTimeDigitRectMmTens()));
    drawDigitDst(renderer, hud,  mm%10,     atlasToFit(timeMap, game_sprites.getTimeDigitRectMmUnits()));
    drawDigitDst(renderer, hud, (ss/10)%10, atlasToFit(timeMap, game_sprites.getTimeDigitRectSsTens()));
    drawDigitDst(renderer, hud,  ss%10,     atlasToFit(timeMap, game_sprites.getTimeDigitRectSsUnits()));
}

void GameWindow::drawSpeedometer(Renderer& renderer, Texture& hud,
                                 int my_player_index, GameStateDTO& last_state,
                                 int hudX, int hudY)
{
    float speed_kmh = last_state.players[my_player_index].state.speed*10;

    const Rect& PANEL_SPEED = game_sprites.getSpeedPanelRect();
    BoxMap spdMap = makeBoxMap(renderer, hud, PANEL_SPEED, hudX, hudY);

    int v = std::clamp((int)std::round(speed_kmh), 0, 999);
    drawDigitDst(renderer, hud, (v/100)%10, atlasToFit(spdMap, game_sprites.getSpeedDigitRectHundreds()));
    drawDigitDst(renderer, hud, (v/10 )%10, atlasToFit(spdMap, game_sprites.getSpeedDigitRectTens()));
    drawDigitDst(renderer, hud,  v%10,      atlasToFit(spdMap, game_sprites.getSpeedDigitRectUnits()));
}


void GameWindow::drawMinimap(Texture& background, Renderer& renderer, GameStateDTO& last_state, Rect& dstRect){

            int bgH = background.GetHeight();
            int bgW = background.GetWidth(); 

            int viewW = dstRect.GetW();

            const int MINIMAP_MARGIN = 12;
            const int MINIMAP_W = 280;
            const float miniScale = (float)MINIMAP_W / (float)bgW;
            const int MINIMAP_H = (int)std::lround(bgH * miniScale);

            const int miniX = viewW - MINIMAP_MARGIN - MINIMAP_W;
            const int miniY = MINIMAP_MARGIN;

            renderer.SetDrawColor(0, 0, 0, 160);
            renderer.FillRect(Rect(miniX - 4, miniY - 4, MINIMAP_W + 8, MINIMAP_H + 8));

            renderer.Copy(
                background,
                Rect(0, 0, bgW, bgH),
                Rect(miniX, miniY, MINIMAP_W, MINIMAP_H)
            );

            struct RGB { Uint8 r,g,b; };
            static const RGB PALETTE[] = {
                {255,  64,  64},
                { 64, 200, 255},
                { 64, 255,  64},
                {255, 220,  64},
                {200,  64, 255},
                {255, 128,   0},
                { 64, 255, 200},
                {255,  64, 180}
            };
            const int DOT_R = 3;

            for (const auto& p : last_state.players) {
                const float world_px = p.state.x * PPM;
                const float world_py = p.state.y * PPM;
                const int mini_px = miniX + (int)std::lround(world_px * miniScale);
                const int mini_py = miniY + (int)std::lround(world_py * miniScale);

                int colorIdx = (int)p.player_id % (int)(sizeof(PALETTE)/sizeof(PALETTE[0]));
                renderer.SetDrawColor(PALETTE[colorIdx].r, PALETTE[colorIdx].g, PALETTE[colorIdx].b, 255);

                renderer.FillRect(Rect(mini_px - DOT_R, mini_py - DOT_R, 2*DOT_R, 2*DOT_R));

                if (p.player_id == client.getMyPlayerId()) {
                    renderer.SetDrawColor(255, 255, 255, 230);
                    renderer.DrawRect(Rect(mini_px - (DOT_R+2), mini_py - (DOT_R+2), 2*(DOT_R+2), 2*(DOT_R+2)));
                }
            }


}

void GameWindow::drawGame(Renderer& renderer,
                         Texture& hud,
                         Texture& background,
                         Texture& sprites,
                         Rect& srcRect,
                         Rect& dstRect,
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
                         int& hp,
                        int& actual_pos, float& pos_x_m, float& pos_y_m, float& angle)
{
    while (true){
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

        renderer.SetDrawColor(0, 0, 0, 255);
        renderer.Clear();

        renderer.Copy(background, srcRect, dstRect);

        for (size_t i = 0; i < last_state.players.size(); i++) {

            if (have_state && last_state.players[i].player_id == client.getMyPlayerId()) {
                my_player_index = i;
                const auto& st = last_state.players[i].state;
                pos_x_m = st.x;
                pos_y_m = st.y;
                angle = st.angle;
                actual_pos = angle_to_frame(angle);

                hp = std::clamp<int>(static_cast<int>(last_state.players[i].health), 0, 100);

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

            const Rect& spr = game_sprites.getCarRect(car_to_use, actual_pos);
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

        const int HUD_PAD      = 2;
        const int HUD_MARGIN_X = 8;
        const int HUD_MARGIN_Y = 8;

        int hudX = HUD_MARGIN_X, hudY = HUD_MARGIN_Y;

        drawHealthBar(renderer, hud, hudX, hudY, hp);

        hudX += BOX_W + HUD_PAD;

        drawSpeedometer(renderer, hud, my_player_index, last_state, hudX, hudY);

        hudX += BOX_W + HUD_PAD;

        drawCronometer(renderer, hud, hudX, hudY, last_state);

        drawMinimap(background, renderer, last_state, dstRect);

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

        Surface hud_surface(DATA_PATH "/assets/hud.png");
        hud_surface.SetColorKey(true, SDL_MapRGB(hud_surface.Get()->format, 255, 201, 14));
        Texture hud(renderer, hud_surface);

        renderer.SetLogicalSize(1600, 800);
            
        std::array<std::string, 3> maps = {
            "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - Liberty City.png",
            "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - San Andreas.png",
            "/cities/Game Boy _ GBC - Grand Theft Auto - Backgrounds - Vice City.png"
        };
        int map_to_play = 0;
        Texture background(renderer, DATA_PATH + maps[map_to_play]);
        const int bgW = background.GetWidth();
        const int bgH = background.GetHeight();

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

        int hp = 0;     

        int my_player_index = 0;
        
        drawGame(renderer,
                      hud,
                      background,
                      sprites,
                      srcRect,
                      dstRect,
                      viewW, viewH,
                      bgW, bgH,
                      rate,
                      perf_freq,
                      t1,
                      it,
                      last_state,
                      have_state,
                      exit,
                      my_player_index,
                      hp,
                      actual_pos, pos_x_m, pos_y_m, angle);
        
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
      receiver(c.getProtocol(), state_queue),
      game_sprites(),
      car_to_use(CarType::ROJO) {}
