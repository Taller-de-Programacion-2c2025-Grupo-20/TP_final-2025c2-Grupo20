#pragma once

#include <string>
#include <stdint.h>

enum class LobbyCommandType : uint8_t {
    LOGIN_ATTEMPT,
    CREATE_MATCH,
    JOIN_MATCH,
    TOGGLE_READY
};

struct LobbyCommand {
    LobbyCommandType type;
    uint8_t client_id; 
    
    std::string username; 
    uint8_t match_id;   
    
    LobbyCommand(LobbyCommandType type, uint8_t id, const std::string& user) :
        type(type), client_id(id), username(user), match_id(0) {}
    
    LobbyCommand(LobbyCommandType type, uint8_t id, uint8_t match) :
        type(type), client_id(id), username(""), match_id(match) {}
    
    LobbyCommand(LobbyCommandType type, uint8_t id) :
        type(type), client_id(id), username(""), match_id(0) {}
};

struct LobbyPlayerInfo {
    uint8_t player_id; 
    std::string name;    
    bool is_ready;      
};