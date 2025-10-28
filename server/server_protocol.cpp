#include "server_protocol.h"

#include <cstring>
#include <utility>

#include <arpa/inet.h>

#include "../common/constants.h"
#include "../common/socket.h"

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

void ServerProtocol::sendResponse(const ResponseDTO& message) {

    std::vector<uint8_t> buffer;
    buffer.push_back(message.command);

    addUint16_tToBuffer(buffer, message.cars_with_nitro);

    buffer.push_back(message.nitro_state);

    skt.sendall(buffer.data(), buffer.size());
}

void ServerProtocol::close() {
    skt.shutdown(SHUT_RDWR);
    skt.close();
}

ServerProtocol::ServerProtocol(Socket&& skt): skt(std::move(skt)) {}

ServerProtocol::~ServerProtocol() {}
