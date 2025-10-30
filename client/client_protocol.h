#include <cstdint>
#include <string>
#include <vector>
#include "../common/socket.h"
#include "../common/serverState.h"
#include "../common/clientCommand.h"


class ClientProtocol {
public:
    ClientProtocol(const char* host, const char* port);
    ~ClientProtocol();

    void ClientProtocol::send_login_attempt(const std::string& username);
    uint8_t receive_login_response(uint8_t& out_player_id);
    void sendInput(const InputCmd& cmd);          

    GameStateDTO ClientProtocol::receive_game_state();

    void close();

private:
    Socket skt;

    void ClientProtocol::addUint16_tToBuffer(std::vector<uint8_t>& buffer, uint16_t value);
    uint16_t receiveUint16_t();
    float receiveFloat();

    ServerState last{};
};
