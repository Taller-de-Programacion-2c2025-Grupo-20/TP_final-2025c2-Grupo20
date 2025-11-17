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
#include "../common/lobbyState.h"
#include "../common/match_list.h"

class ServerProtocol {
private:
    Socket skt;

    void addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value);
    void addFloatToBuffer(std::vector<uint8_t>& buffer, float value);
    uint16_t receiveUint16_t();
    std::string receiveString();

public:
    uint8_t receiveCommand();
    uint8_t receiveUint8_t();
    void send_game_state(const GameStateDTO& state);
    std::string receive_login_attempt();
    InputCmd receive_input_command();
    std::string receive_create_match_payload();
    void send_login_ok(uint8_t player_id);
    void send_lobby_state(const LobbyStateDTO& state);
    void send_match_list(const MatchListDTO& list);

    void close();

    explicit ServerProtocol(Socket&& skt);

    ServerProtocol(const ServerProtocol&) = delete;
    ServerProtocol& operator=(const ServerProtocol&) = delete;

    ~ServerProtocol();
};

#endif
