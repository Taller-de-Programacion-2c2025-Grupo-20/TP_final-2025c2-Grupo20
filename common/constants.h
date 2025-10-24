#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>

//Constantes TP FINAL

enum class InputAction : uint8_t { Press, Release };
enum class InputKey    : uint8_t { Up, Down, Left, Right, Quit};

//Constantes VIEJAS
struct Instruction {
    uint8_t command;
    int client_id;

    Instruction(): command(0x0), client_id(-1) {}
    Instruction(uint8_t command, int client_id): command(command), client_id(client_id) {}
};

struct ResponseDTO {
    uint8_t command;
    uint16_t cars_with_nitro;

    uint8_t nitro_state;

    ResponseDTO(): command(0x0), cars_with_nitro(0), nitro_state(0) {}

    ResponseDTO(uint8_t command, uint16_t cars_with_nitro, uint8_t nitro_state):
            command(command), cars_with_nitro(cars_with_nitro), nitro_state(nitro_state) {}
};

#define SEND_NITRO 0x04
#define SEND_CARS_WITH_NITRO 0x10
#define NITRO_ACTIVATED 0x07
#define NITRO_DEACTIVATED 0x08

#endif
