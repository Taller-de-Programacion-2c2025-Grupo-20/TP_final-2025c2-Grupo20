#ifndef CLIENT_PROTOCOL_H
#define CLIENT_PROTOCOL_H

#include <string>
#include <vector>
#include <stdint.h>

#include "../common/socket.h"
#include "../common/clientCommand.h"
#include "../common/gameState.h"
#include "../common/lobbyState.h"
#include "../common/match_list.h"

class ClientProtocol {
private:
    Socket skt;

    void addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value);
    uint8_t receiveUint8_t();
    uint16_t receiveUint16_t();
    std::string receiveString();
    float receiveFloat();

public:
    ClientProtocol(const char* hostname, const char* servname);
    ~ClientProtocol();

    void send_login_attempt(const std::string& username);
    void send_create_match(const std::string& match_name);
    void send_join_match(uint8_t match_id);
    void send_start_game();
    void send_input(const InputCmd& cmd);

    bool receive_command_code(uint8_t& code);
    uint8_t receive_login_response_payload();
    LobbyStateDTO receive_lobby_state_payload();
    GameStateDTO receive_game_state_payload();
    MatchListDTO receive_match_list_payload();
    
    void close();

    ClientProtocol(const ClientProtocol&) = delete;
    ClientProtocol& operator=(const ClientProtocol&) = delete;
};

#endif