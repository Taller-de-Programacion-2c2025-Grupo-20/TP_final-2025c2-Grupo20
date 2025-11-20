#ifndef CLIENTCOMMAND_H_
#define CLIENTCOMMAND_H_
#include <cstdint>
#include "constants.h"

//Struct a utilizar para enviar comandos
struct InputCmd {
    uint8_t player_id;
    InputKey key;
    InputAction action;

    std::string username;
    uint8_t match_id;

    explicit InputCmd() : player_id(0), key(InputKey::Up), action(InputAction::Press) {}
};

#endif