#pragma once

#include <string>
#include <stdint.h>

enum class LobbyCommandType : uint8_t {
    LOGIN_ATTEMPT,
    CREATE_MATCH,
    JOIN_MATCH,
    START_GAME,
    REFRESH_MATCH_LIST
};

struct LobbyCommand {
    LobbyCommandType type;
    uint8_t client_id; 
    
    std::string text_payload; 
    uint8_t id_payload;   

    LobbyCommand() {}

    LobbyCommand(LobbyCommandType type, uint8_t id, const std::string& text) :
        type(type), client_id(id), text_payload(text), id_payload(0) {}
    
    LobbyCommand(LobbyCommandType type, uint8_t id, uint8_t numeric_id) :
        type(type), client_id(id), text_payload(""), id_payload(numeric_id) {}
    
    LobbyCommand(LobbyCommandType type, uint8_t id) :
        type(type), client_id(id), text_payload(""), id_payload(0) {}
};

