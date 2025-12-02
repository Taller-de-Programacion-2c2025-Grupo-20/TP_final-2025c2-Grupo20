#pragma once
#include <string>
#include <vector>

#include <stdint.h>

struct MatchInfo {
    uint8_t match_id;
    std::string name;
    uint8_t player_count;
};

struct MatchListDTO {
    std::vector<MatchInfo> matches;
};