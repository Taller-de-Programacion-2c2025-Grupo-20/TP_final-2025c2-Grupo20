// clientProtocol.cpp
#include "client_protocol.h"
#include <cmath>

namespace {
    constexpr float    SPEED_PX_PER_SEC = 300.0f;  
    constexpr uint32_t ROT_EVERY_MS     = 80;     
    inline int wrap16(int v) { return v & 15; }    
}

ClientProtocol::ClientProtocol(const char* /*host*/, const char* /*port*/) {
    // Estado inicial
    x = 0.f;
    y = 0.f;
    sprite_idx = 0;
    last = {0, x, y, sprite_idx};
}

std::array<float,2> ClientProtocol::getInitialPos() const {
    return {x, y};
}

void ClientProtocol::sendInput(const InputCmd& cmd) {
    const bool pressed = (cmd.action == InputAction::Press);

    switch (cmd.key) {
        case InputKey::Quit:

            break;

        case InputKey::Up:
            if (pressed) { running = true;  backwards = false; }
            else         { running = false; }
            break;

        case InputKey::Down:
            if (pressed) { running = true;  backwards = true;  }
            else         { running = false; backwards = false; }
            break;

        case InputKey::Right:
            turn_dir = pressed ? +1 : 0;
            break;

        case InputKey::Left:
            turn_dir = pressed ? -1 : 0;
            break;

        default:
            break;
    }
}

void ClientProtocol::serverTick(uint32_t dt_ms) {

    rot_acc_ms += dt_ms;
    while (rot_acc_ms >= ROT_EVERY_MS) {
        if (turn_dir == +1)      sprite_idx = wrap16(sprite_idx + 1);
        else if (turn_dir == -1) sprite_idx = wrap16(sprite_idx + 15);
        rot_acc_ms -= ROT_EVERY_MS;
    }

    if (running) {
        float vx = dir[sprite_idx][0];
        float vy = dir[sprite_idx][1];
        if (backwards) { vx = -vx; vy = -vy; }

        const float dt_s = dt_ms / 1000.0f;
        x += vx * SPEED_PX_PER_SEC * dt_s;
        y -= vy * SPEED_PX_PER_SEC * dt_s; 
    }

    server_time_ms += dt_ms;
    last = { server_time_ms, x, y, sprite_idx };
}

std::optional<ServerState> ClientProtocol::pollState() {

    return last;
}
