#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <cstdint>

#include "../common/constants.h"
#include "../common/socket.h"

class ClientProtocol {
private:
    Socket skt;


public:

    ClientProtocol(const char* hostname, const char* port);

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;

    ~ClientProtocol();
};

#endif
