// clientProtocol.h  (mínimo necesario para esta implementación)
#pragma once
#include <array>
#include <optional>
#include <cstdint>

enum class InputAction : uint8_t { Press, Release };
enum class InputKey    : uint8_t { Up, Down, Left, Right, Quit, Unknown };

struct InputCmd {
    uint32_t player_id{0};
    InputKey key{InputKey::Unknown};
    InputAction action{InputAction::Press};
    uint64_t client_ts_ms{0};
};

struct ServerState {
    uint64_t server_ts_ms{0};
    float x{0.f};
    float y{0.f};
    int   sprite_idx{0}; 
};

class ClientProtocol {
public:
    ClientProtocol(const char* host, const char* port);

    std::array<float,2> getInitialPos() const;

    void sendInput(const InputCmd& cmd);          
    void serverTick(uint32_t dt_ms);              
    std::optional<ServerState> pollState();      

private:
    bool running = false;
    bool backwards = false;
    int  turn_dir = 0;       

    float x = 0.f, y = 0.f;
    int   sprite_idx = 0;    

    std::array<std::array<float,2>, 16> dir = {{
        {{ 0.4f,  0.0f}}, {{ 0.3f, -0.1f}}, {{ 0.2f, -0.2f}}, {{ 0.1f, -0.3f}},
        {{ 0.0f, -0.4f}}, {{-0.1f, -0.3f}}, {{-0.2f, -0.2f}}, {{-0.3f, -0.1f}},
        {{-0.4f,  0.0f}}, {{-0.3f,  0.1f}}, {{-0.2f,  0.2f}}, {{-0.1f,  0.3f}},
        {{ 0.0f,  0.4f}}, {{ 0.1f,  0.3f}}, {{ 0.2f,  0.2f}}, {{ 0.3f,  0.1f}}
    }};

    uint64_t server_time_ms = 0;
    uint32_t rot_acc_ms = 0;
    ServerState last{};
};
