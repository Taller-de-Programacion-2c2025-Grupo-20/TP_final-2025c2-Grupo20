#pragma once
#include <vector>
#include <set>
#include <stdint.h>
#include "serverState.h"
#include "constants.h"

struct PlayerState {
    uint8_t player_id;
    ServerState state;
    uint8_t health;

    CarType car_type;

    std::set<UpgradeType> applied_upgrades;

    float next_checkpoint_position_x;
    float next_checkpoint_position_y;
    float next_checkpoint_hint;
};

struct GameStateDTO {
    uint8_t car_count;
    float elapsed_time;
    std::vector<PlayerState> players;
};