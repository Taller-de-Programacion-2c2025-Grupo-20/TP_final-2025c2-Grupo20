#include "client_protocol.h"

ClientProtocol::ClientProtocol(const char* hostname, const char* servname):
        skt(hostname, servname) {}

ClientProtocol::~ClientProtocol() {}
