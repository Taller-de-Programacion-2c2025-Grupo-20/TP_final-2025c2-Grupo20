#include "server_protocol.h"

#include <cstring>
#include <utility>

#include <arpa/inet.h>

#include "../common/constants.h"
#include "../common/socket.h"
#include "../common/gameState.h"
#include "../common/match_list.h"
#include <iostream>

uint8_t ServerProtocol::receiveCommand() {
    uint8_t command;
    if (skt.recvall(&command, sizeof(uint8_t)) == 0) {
        return 0x0;
    }

    return command;
}

void ServerProtocol::addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value) {
    uint16_t value_big_endian = htons(value);

    size_t buffer_size = buffer.size();
    buffer.resize(buffer_size + sizeof(uint16_t));
    memcpy(buffer.data() + buffer_size, &value_big_endian, sizeof(uint16_t));
}

uint16_t ServerProtocol::receiveUint16_t() {
    uint16_t value_big_endian;
    skt.recvall(&value_big_endian, sizeof(uint16_t));
    return ntohs(value_big_endian);
}

uint8_t ServerProtocol::receiveUint8_t() {
    uint8_t byte;
    skt.recvall(&byte, sizeof(uint8_t));
    return byte;
}

std::string ServerProtocol::receiveString() {
    uint16_t len = receiveUint16_t();
    std::vector<char> buffer(len);
    if (len > 0) {
        skt.recvall(buffer.data(), len);
    }
    return std::string(buffer.begin(), buffer.end());
}

std::string ServerProtocol::receive_create_match_payload() {
    return receiveString();
}

void ServerProtocol::close() {
    skt.shutdown(SHUT_RDWR);
    skt.close();
}

void ServerProtocol::addFloatToBuffer(std::vector<uint8_t>& buffer, float value) {
    uint32_t temp_int;
    std::memcpy(&temp_int, &value, sizeof(float));
    uint32_t network_value = htonl(temp_int);
    
    size_t old_size = buffer.size();
    buffer.resize(old_size + sizeof(uint32_t));
    std::memcpy(buffer.data() + old_size, &network_value, sizeof(uint32_t));
}

void ServerProtocol::send_game_state(const GameStateDTO& state) {
    std::vector<uint8_t> buffer;
    buffer.push_back(EVT_GAME_STATE); 
    buffer.push_back(state.car_count);
    
    addFloatToBuffer(buffer, state.elapsed_time);

    for (const auto& player : state.players) {
        buffer.push_back(player.player_id);
        
        addFloatToBuffer(buffer, player.state.x);
        addFloatToBuffer(buffer, player.state.y);
        addFloatToBuffer(buffer, player.state.angle);
        addFloatToBuffer(buffer, player.state.speed);
        
        buffer.push_back(player.health);
    }

    skt.sendall(buffer.data(), buffer.size());
}

std::string ServerProtocol::receive_login_attempt() {
    return receiveString();
}

InputCmd ServerProtocol::receive_input_command() {
    InputCmd cmd;
    uint8_t action, key, id;
    
    skt.recvall(&action, 1);
    skt.recvall(&key, 1);
    skt.recvall(&id, 1);
    
    cmd.action = static_cast<InputAction>(action);
    cmd.key = static_cast<InputKey>(key);
    cmd.player_id = static_cast<u_int8_t>(id);

    return cmd;
}

void ServerProtocol::send_login_ok(uint8_t player_id) {
    std::vector<uint8_t> buffer;
    buffer.push_back(LOGIN_SUCCESS);   // mismo código que espera el cliente
    buffer.push_back(player_id);   // id asignado al cliente
    skt.sendall(buffer.data(), buffer.size());
}

void ServerProtocol::send_lobby_state(const LobbyStateDTO& state) {
    std::vector<uint8_t> buffer;
    
    buffer.push_back(RSP_LOBBY_STATE);
    buffer.push_back(static_cast<uint8_t>(state.players.size()));
    buffer.push_back(state.host_id);
    buffer.push_back(state.map_id);

    for (const auto& player : state.players) {
        buffer.push_back(player.player_id);
        
        uint16_t name_len = player.name.length();
        addUint16_tToBuffer(buffer, name_len); 
        buffer.insert(buffer.end(), player.name.begin(), player.name.end());
    }
    
    skt.sendall(buffer.data(), buffer.size());
}

void ServerProtocol::send_match_list(const MatchListDTO& list) {
    std::vector<uint8_t> buffer;
    
    buffer.push_back(RSP_MATCH_LIST);
    buffer.push_back(static_cast<uint8_t>(list.matches.size()));

    for (const auto& match : list.matches) {
        buffer.push_back(match.match_id);
        buffer.push_back(match.player_count); 
        uint16_t name_len = match.name.length();
        addUint16_tToBuffer(buffer, name_len); 
        buffer.insert(buffer.end(), match.name.begin(), match.name.end());
    }
    
    skt.sendall(buffer.data(), buffer.size());
}

void ServerProtocol::send_start_game() {
    uint8_t cmd = EVT_GAME_STARTED;
    skt.sendall(&cmd, sizeof(uint8_t));
}

ServerProtocol::ServerProtocol(Socket&& skt): skt(std::move(skt)) {}

ServerProtocol::~ServerProtocol() {}
