#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../common_src/constants.h"
#include "../common_src/socket.h"

class ServerProtocol {
private:
    Socket skt;

    void addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value);

public:
    uint8_t receiveCommand();
    void sendResponse(const ResponseDTO& message);

    void close();

    explicit ServerProtocol(Socket&& skt);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ~ServerProtocol();
};

#endif
