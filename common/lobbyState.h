#pragma once
#include <cstdint>
#include <string>
#include <vector>

/**
 * @brief Representa la información de un solo jugador
 * en la pantalla del lobby.
 */
struct LobbyPlayerInfo {
    uint8_t player_id;  
    std::string name;
    uint8_t car_id;
};

/**
 * @brief El paquete completo que el servidor envía (broadcast)
 * a todos los clientes en el lobby cada vez que algo cambia.
 */
struct LobbyStateDTO {
    std::vector<LobbyPlayerInfo> players;
    uint8_t host_id;
    uint8_t map_id;
};
