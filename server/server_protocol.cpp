#include "server_protocol.h"

#include <cstring>
#include <utility>

#include <arpa/inet.h>

#include "../common/constants.h"
#include "../common/socket.h"
#include "../common/gameState.h"

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
    
    for (const auto& player : state.players) {
        buffer.push_back(player.player_id);
        
        addFloatToBuffer(buffer, player.state.x);
        addFloatToBuffer(buffer, player.state.y);
        addFloatToBuffer(buffer, player.state.angle);
        
        buffer.push_back(player.health);
    }
    
    skt.sendall(buffer.data(), buffer.size());
}

std::string ServerProtocol::receive_login_attempt() {
    uint16_t name_length = receiveUint16_t();
    
    std::vector<char> buffer(name_length);
    if (name_length > 0) {
        skt.recvall(buffer.data(), name_length);
    }
    return std::string(buffer.begin(), buffer.end());
}

InputCmd ServerProtocol::receive_input_command() {
    InputCmd cmd;
    uint8_t action, key;
    
    skt.recvall(&action, 1);
    skt.recvall(&key, 1);
    
    cmd.action = static_cast<InputAction>(action);
    cmd.key = static_cast<InputKey>(key);
    return cmd;
}

void ServerProtocol::send_login_ok(uint8_t player_id) {
    std::vector<uint8_t> buffer;
    buffer.push_back(CMD_LOGIN);   // mismo código que espera el cliente
    buffer.push_back(player_id);   // id asignado al cliente
    skt.sendall(buffer.data(), buffer.size());
}

ServerProtocol::ServerProtocol(Socket&& skt): skt(std::move(skt)) {}

ServerProtocol::~ServerProtocol() {}
