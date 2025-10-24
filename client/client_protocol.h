#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>

#include "../common_src/constants.h"
#include "../common_src/socket.h"

class ClientProtocol {
private:
    Socket skt;

    void sendCommand(uint8_t command);
    uint32_t receiveBuffer();

public:
    void sendNitro();
    ResponseDTO receiveMessage();

    ClientProtocol(const char* hostname, const char* port);

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ~ClientProtocol();
};

#endif
