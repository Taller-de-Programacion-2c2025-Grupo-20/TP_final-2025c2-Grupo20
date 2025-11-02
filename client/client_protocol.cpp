#include "client_protocol.h"
#include <cmath>
#include <cstring>
#include <arpa/inet.h>
#include "../common/constants.h"
#include "../common/gameState.h"


ClientProtocol::ClientProtocol(const char* host, const char* port) : skt(host, port) {}

void ClientProtocol::addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value) {
    uint16_t value_big_endian = htons(value);
    size_t buffer_size = buffer.size();
    buffer.resize(buffer_size + sizeof(uint16_t));
    memcpy(buffer.data() + buffer_size, &value_big_endian, sizeof(uint16_t));
}

uint16_t ClientProtocol::receiveUint16_t() {
    uint16_t value_big_endian;
    skt.recvall(&value_big_endian, sizeof(uint16_t));
    return ntohs(value_big_endian);
}

void ClientProtocol::send_login_attempt(const std::string& username) {
    std::vector<uint8_t> buffer;

    uint8_t cmd = CMD_LOGIN;
    buffer.push_back(cmd);

    uint16_t name_length = username.length();
    addUint16_tToBuffer(buffer, name_length);

    buffer.insert(buffer.end(), username.data(), username.data() + name_length);
    skt.sendall(buffer.data(), buffer.size());
}

uint8_t ClientProtocol::receive_login_response(uint8_t& out_player_id) {
    uint8_t response_code;
    skt.recvall(&response_code, 1);
    
    if (response_code == CMD_LOGIN) { 
        skt.recvall(&out_player_id, 1);
    }
    return response_code;
}

void ClientProtocol::sendInput(const InputCmd& cmd) {
    std::vector<uint8_t> buffer;
    uint8_t cmd_byte = CMD_ENVIAR_INPUT;
    buffer.push_back(cmd_byte);
    buffer.push_back(static_cast<uint8_t>(cmd.action));
    buffer.push_back(static_cast<uint8_t>(cmd.key));
    
    skt.sendall(buffer.data(), buffer.size());
}

float ClientProtocol::receiveFloat() {
    uint32_t network_value;
    skt.recvall(&network_value, sizeof(uint32_t));
    uint32_t host_value = ntohl(network_value);
    float value;
    std::memcpy(&value, &host_value, sizeof(float));
    return value;
}

GameStateDTO ClientProtocol::receive_game_state() {
    GameStateDTO state_dto;

    //1. Leer Comando (asumiendo que ya se leyó o se sabe)
    uint8_t cmd_code;
    skt.recvall(&cmd_code, 1);

    uint8_t car_count;
    skt.recvall(&car_count, 1);
    state_dto.car_count = car_count;

    for (int i = 0; i < car_count; ++i) {
        PlayerState player;
        
        skt.recvall(&player.player_id, 1);
        
        player.state.x = receiveFloat();
        player.state.y = receiveFloat();
        player.state.angle = receiveFloat();
        
        skt.recvall(&player.health, 1);
        
        state_dto.players.push_back(player);
    }
    
    return state_dto;
}

void ClientProtocol::close() {
    skt.shutdown(SHUT_RDWR);
    skt.close();
}

ClientProtocol::~ClientProtocol() {}