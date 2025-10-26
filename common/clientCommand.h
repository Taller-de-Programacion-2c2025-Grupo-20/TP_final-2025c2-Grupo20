#ifndef CLIENTCOMMAND_H_
#define CLIENTCOMMAND_H_
#include <cstdint>
#include <constants.h>

//Struct a utilizar para enviar comandos
struct InputCmd {
    uint32_t player_id;
    InputKey key;
    InputAction action;

    explicit InputCmd() : player_id(0), key(InputKey::Up), action(InputAction::Press) {}
};

#endif