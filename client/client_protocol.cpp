#include "client_protocol.h"

#include <cstring>

#include "../common_src/constants.h"

uint32_t ClientProtocol::receiveBuffer() {

    uint32_t buffer;

    skt.recvall(&buffer, sizeof(buffer));

    return buffer;
}

ResponseDTO ClientProtocol::receiveMessage() {
    uint32_t buffer = receiveBuffer();
    uint8_t command;
    uint8_t nitro_state;

    memcpy(&command, &buffer, sizeof(command));
    if (command != SEND_CARS_WITH_NITRO)
        return ResponseDTO();

    memcpy(&nitro_state, reinterpret_cast<uint8_t*>(&buffer) + 3, sizeof(nitro_state));

    return ResponseDTO(command, 0, nitro_state);
}

void ClientProtocol::sendCommand(uint8_t command) { skt.sendall(&command, sizeof(uint8_t)); }

void ClientProtocol::sendNitro() { sendCommand(SEND_NITRO); }

ClientProtocol::ClientProtocol(const char* hostname, const char* servname):
        skt(hostname, servname) {}

ClientProtocol::~ClientProtocol() {}
