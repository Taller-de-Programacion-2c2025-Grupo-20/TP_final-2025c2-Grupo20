#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>

//Constantes TP FINAL

enum class InputAction : uint8_t { Press, Release };
enum class InputKey    : uint8_t { Up, Down, Left, Right, Quit, CreateMatch, JoinMatch, StartGame, Unknown };
enum class EntityType  : uint8_t { CAR, WALL, CHECKPOINT };
enum class CarType  : uint8_t {VERDE, ROJO, DESCAPOTABLE, CELESTE, JEEP, CAMIONETA, CAMION};


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


const uint8_t CMD_LOGIN = 0x01;
const uint8_t LOGIN_SUCCESS = 0x2;
const uint8_t LOGIN_FAILED = 0x03;
const uint8_t CMD_ENVIAR_INPUT = 0x04;
const uint8_t CMD_CREATE_MATCH = 0x05;
const uint8_t CMD_JOIN_MATCH = 0x06;
const uint8_t CMD_START_GAME = 0x07;
const uint8_t CMD_REFRESH_MATCH_LIST = 0x08;

const uint8_t EVT_GAME_STARTED = 0x70;
const uint8_t EVT_GAME_STATE = 0x80;
const uint8_t EVT_RACE_FINISHED = 0x90;
const uint8_t RSP_LOBBY_STATE = 0x91;
const uint8_t RSP_MATCH_LIST = 0x92;

#endif
