#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../common/constants.h"
#include "../common/socket.h"
#include "../common/gameState.h"
#include "../common/clientCommand.h"

class ServerProtocol {
private:
    Socket skt;

    void addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value);
    void addFloatToBuffer(std::vector<uint8_t>& buffer, float value);
    uint16_t receiveUint16_t();

public:
    uint8_t receiveCommand();
    void send_game_state(const GameStateDTO& state);
    std::string receive_login_attempt();
    InputCmd receive_input_command();
    void send_login_ok(uint8_t player_id);

    void close();

    explicit ServerProtocol(Socket&& skt);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ~ServerProtocol();
};

#endif
