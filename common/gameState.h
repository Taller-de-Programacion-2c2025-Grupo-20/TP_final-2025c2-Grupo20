#pragma once
#include <set>
#include <vector>

#include <stdint.h>

#include "constants.h"
#include "serverState.h"

struct PlayerState {
    uint8_t player_id;
    ServerState state;
    uint8_t health;

    CarType car_type;

    std::set<UpgradeType> applied_upgrades;

    float next_checkpoint_position_x;
    float next_checkpoint_position_y;
    float next_checkpoint_hint;

    uint8_t checkpoints_passed;
};

struct PlayerResultDTO {
    uint8_t player_id;
    uint8_t position;
    float total_time;
};

struct GameStateDTO {
    uint8_t car_count;
    float elapsed_time;
    std::vector<PlayerState> players;
    uint8_t race_finished = 0;
    std::vector<PlayerResultDTO> final_results;
};