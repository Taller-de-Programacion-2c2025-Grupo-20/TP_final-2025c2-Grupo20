#include "client_protocol.h"
#include <cmath>
#include <cstring>
#include <arpa/inet.h>
#include "../common/constants.h"
#include "../common/gameState.h"
#include <iostream>
#include <vector>

ClientProtocol::ClientProtocol(const char* host, const char* port) : skt(host, port) {}
ClientProtocol::~ClientProtocol() {}


void ClientProtocol::send_login_attempt(const std::string& username) {
    std::vector<uint8_t> buffer;
    uint8_t cmd = CMD_LOGIN;
    buffer.push_back(cmd);
    uint16_t name_length = username.length();
    addUint16_tToBuffer(buffer, name_length);
    buffer.insert(buffer.end(), username.data(), username.data() + name_length);
    skt.sendall(buffer.data(), buffer.size());
}

void ClientProtocol::send_create_match(const std::string& match_name) {
    std::vector<uint8_t> buffer;
    buffer.push_back(CMD_CREATE_MATCH);
    sendString(match_name);
}

void ClientProtocol::send_join_match(uint8_t match_id) {
    std::vector<uint8_t> buffer;
    buffer.push_back(CMD_JOIN_MATCH);
    buffer.push_back(match_id);
    skt.sendall(buffer.data(), buffer.size());
}

void ClientProtocol::send_toggle_ready() {
    std::vector<uint8_t> buffer;
    buffer.push_back(CMD_TOGGLE_READY);
    skt.sendall(buffer.data(), buffer.size());
}

void ClientProtocol::send_input(const InputCmd& cmd) {
    std::vector<uint8_t> buffer;
    buffer.push_back(CMD_ENVIAR_INPUT);
    buffer.push_back(static_cast<uint8_t>(cmd.action));
    buffer.push_back(static_cast<uint8_t>(cmd.key));
    buffer.push_back(static_cast<uint8_t>(cmd.player_id)); 
    
    skt.sendall(buffer.data(), buffer.size());
}


bool ClientProtocol::receive_command_code(uint8_t& code) {
    return (skt.recvall(&code, 1) > 0);
}

uint8_t ClientProtocol::receive_login_response_payload() {
    return receiveUint8_t();
}

LobbyStateDTO ClientProtocol::receive_lobby_state_payload() {
    LobbyStateDTO state;
    
    uint8_t player_count = receiveUint8_t();
    state.host_id = receiveUint8_t();
    state.map_id = receiveUint8_t();

    for (int i = 0; i < player_count; ++i) {
        LobbyPlayerInfo player;
        player.player_id = receiveUint8_t();
        player.is_ready = (receiveUint8_t() == 1);
        player.name = receiveString(); 
        state.players.push_back(player);
    }
    return state;
}

GameStateDTO ClientProtocol::receive_game_state_payload() {
    GameStateDTO state_dto;

    uint8_t car_count;
    skt.recvall(&car_count, 1);
    state_dto.car_count = car_count;

    state_dto.elapsed_time = receiveFloat();

    for (int i = 0; i < car_count; ++i) {
        PlayerState player;
        
        skt.recvall(&player.player_id, 1);
        
        player.state.x = receiveFloat();
        player.state.y = receiveFloat();
        player.state.angle = receiveFloat();
        player.state.speed = receiveFloat();
        
        skt.recvall(&player.health, 1);
        
        state_dto.players.push_back(player);
    }
    
    return state_dto;
}

void ClientProtocol::addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value) {
    uint16_t value_big_endian = htons(value);
    size_t buffer_size = buffer.size();
    buffer.resize(buffer_size + sizeof(uint16_t));
    memcpy(buffer.data() + buffer_size, &value_big_endian, sizeof(uint16_t));
}

void ClientProtocol::sendString(const std::string& s) {
    std::vector<uint8_t> buffer;
    uint16_t len = s.length();
    addUint16_tToBuffer(buffer, len);
    buffer.insert(buffer.end(), s.begin(), s.end());
    skt.sendall(buffer.data(), buffer.size());
}

uint8_t ClientProtocol::receiveUint8_t() {
    uint8_t byte;
    skt.recvall(&byte, 1);
    return byte;
}

uint16_t ClientProtocol::receiveUint16_t() {
    uint16_t value_big_endian;
    skt.recvall(&value_big_endian, sizeof(uint16_t));
    return ntohs(value_big_endian);
}

std::string ClientProtocol::receiveString() {
    uint16_t len = receiveUint16_t();
    std::vector<char> buffer(len);
    if (len > 0) {
        skt.recvall(buffer.data(), len);
    }
    return std::string(buffer.begin(), buffer.end());
}

float ClientProtocol::receiveFloat() {
    uint32_t network_value;
    skt.recvall(&network_value, sizeof(uint32_t));
    uint32_t host_value = ntohl(network_value);
    float value;
    std::memcpy(&value, &host_value, sizeof(float));
    return value;
}

void ClientProtocol::close() {
    skt.shutdown(SHUT_RDWR);
    skt.close();
}