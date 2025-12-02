#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>
#include <string>

//Constantes TP FINAL

enum class InputAction : uint8_t { Press, Release };
enum class InputKey    : uint8_t { Up, Down, Left, Right, Quit, CreateMatch, JoinMatch, StartGame, SelectCar, SelectMap, Unknown, BuySpeedUpgrade, BuyAccelerationUpgrade, BuyHealthUpgrade};
enum class EntityType  : uint8_t { CAR, WALL, CHECKPOINT };
enum class CarType     : uint8_t {VERDE, ROJO, DESCAPOTABLE, CELESTE, JEEP, CAMIONETA, CAMION};
enum class UpgradeType : uint8_t { SpeedUpgrade, AccelerationUpgrade, HealthUpgrade };


// Constantes LobbyScreen
const int TOTAL_CARS = 7;
const int TOTAL_MAPS = 3;
const float maxSens = 14.0f;
const std::string LIBERTY_CITY = "liberty_city.yaml";
const std::string VICE_CITY = "vice_city.yaml";
const std::string SAN_ANDREAS = "san_andreas.yaml";

inline const char* LIBERTY = "Liberty City";
inline const char* VICE = "Vice City";
inline const char* ANDREAS = "San Andreas";

inline const char* LIBERTY_CITY_FILE = "liberty_city.png";
inline const char* VICE_CITY_FILE = "vice_city.png";
inline const char* SAN_ANDREAS_FILE = "san_andreas.png";
inline const char* CARS_FILE = "Mobile - Grand Theft Auto 4 - Miscellaneous - Cars.png";
inline const char* FONT_FILE = ":/data/font/PressStart2P-Regular.ttf";
inline const char* LOBBY_FILE = ":/Pantallas/fondo_lobby.png";
inline const char* GARAGE_FILE = ":/Pantallas/fondo_garage.png";
inline const char* SELECT_MAP_FILE = ":/Pantallas/fondo_select_map.png";
inline const char* RESULT_FILE = ":/Pantallas/fondo_result.png";

inline const char* VERDE = "Verde";
inline const char* ROJO = "Rojo";
inline const char* DESCAPOTABLE = "Descapotable";
inline const char* CELESTE = "Celeste";
inline const char* JEEP = "Jeep";
inline const char* CAMIONETA = "Camioneta";
inline const char* CAMION = "Camión";

inline const char* BACKGROUND_STYLE = "background: transparent;";

inline constexpr float MATCH_DURATION_SECONDS = 615.0f;
inline constexpr int BUY_TIME_SECONDS = 15;


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
const uint8_t CMD_SELECT_MAP = 0x09;
const uint8_t CMD_SELECT_CAR = 0x0A;

const uint8_t EVT_GAME_STARTED = 0x70;
const uint8_t EVT_GAME_STATE = 0x80;
const uint8_t EVT_RACE_FINISHED = 0x90;
const uint8_t RSP_LOBBY_STATE = 0x91;
const uint8_t RSP_MATCH_LIST = 0x92;

#endif
