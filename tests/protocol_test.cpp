#include <gtest/gtest.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <thread>

#include "../client/client_protocol.h"
#include "../server/server_protocol.h"
#include "../common/socket.h"
#include "../common/gameState.h"
#include "../common/constants.h"

class ProtocolTest : public ::testing::Test {
protected:
    std::unique_ptr<ClientProtocol> clientProto;
    std::unique_ptr<ServerProtocol> serverProto;
    
    std::thread acceptorThread;

    void SetUp() override {
        const char* port = "8085"; 
        const char* ip = "127.0.0.1";

        Socket listener(port);
        Socket* serverSideSocketRaw = nullptr;
        
        acceptorThread = std::thread([&listener, &serverSideSocketRaw]() {
            try {
                serverSideSocketRaw = new Socket(listener.accept());
            } catch (...) {
                // ...
            }
        });
        Socket clientSideSocket(ip, port);

        if (acceptorThread.joinable()) {
            acceptorThread.join();
        }

        if (serverSideSocketRaw) {
            serverProto = std::make_unique<ServerProtocol>(std::move(*serverSideSocketRaw));
            delete serverSideSocketRaw; 
        }
        
        clientProto = std::make_unique<ClientProtocol>(std::move(clientSideSocket));
    }
    
};

// TEST 1: Login Success
TEST_F(ProtocolTest, LoginAttempt) {
    std::string username = "JugadorPro123";
    
    clientProto->send_login_attempt(username);

    uint8_t cmd = serverProto->receiveCommand();
    ASSERT_EQ(cmd, CMD_LOGIN);
    
    std::string receivedName = serverProto->receive_login_attempt();
    EXPECT_EQ(username, receivedName);
}

// TEST 2: Login Failed
TEST_F(ProtocolTest, LoginFailed) {
    serverProto->send_login_failed();

    uint8_t cmd;
    bool received = clientProto->receive_command_code(cmd);
    
    ASSERT_TRUE(received);
    EXPECT_EQ(cmd, LOGIN_FAILED);
}

// TEST 3: Crear Partida
TEST_F(ProtocolTest, CreateMatch) {
    std::string matchName = "Carrera Mortal";
    
    clientProto->send_create_match(matchName);

    uint8_t cmd = serverProto->receiveCommand();
    ASSERT_EQ(cmd, CMD_CREATE_MATCH);

    std::string receivedMatchName = serverProto->receive_create_match_payload();
    EXPECT_EQ(matchName, receivedMatchName);
}

// TEST 4: Input de Juego
TEST_F(ProtocolTest, SendInput) {
    InputCmd originalCmd;
    originalCmd.action = InputAction::Press;
    originalCmd.key = InputKey::Up;
    originalCmd.player_id = 5;

    clientProto->send_input(originalCmd);

    uint8_t cmd = serverProto->receiveCommand();
    ASSERT_EQ(cmd, CMD_ENVIAR_INPUT);

    InputCmd receivedCmd = serverProto->receive_input_command();
    
    EXPECT_EQ((int)receivedCmd.action, (int)originalCmd.action);
    EXPECT_EQ((int)receivedCmd.key, (int)originalCmd.key);
    EXPECT_EQ((int)receivedCmd.player_id, (int)originalCmd.player_id);
}

// TEST 5: GameState en Carrera
TEST_F(ProtocolTest, GameStateRunning) {
    GameStateDTO state;
    state.car_count = 1;
    state.elapsed_time = 10.5f;
    state.race_finished = 0;

    PlayerState p1;
    p1.player_id = 1;
    p1.state = {100.5f, 200.5f, 90.0f, 50.0f};
    p1.health = 80;
    p1.car_type = CarType::ROJO;
    p1.checkpoints_passed = 2;
    p1.next_checkpoint_position_x = 300.0f;
    p1.next_checkpoint_position_y = 400.0f;
    p1.next_checkpoint_hint = 45.0f;
    p1.applied_upgrades.insert(UpgradeType::SpeedUpgrade);

    state.players.push_back(p1);

    serverProto->send_game_state(state);

    uint8_t cmd;
    bool ok = clientProto->receive_command_code(cmd);
    ASSERT_TRUE(ok);
    ASSERT_EQ(cmd, EVT_GAME_STATE);

    GameStateDTO receivedState = clientProto->receive_game_state_payload();

    EXPECT_FLOAT_EQ(receivedState.elapsed_time, state.elapsed_time);
    EXPECT_EQ(receivedState.players.size(), 1);
    EXPECT_EQ(receivedState.players[0].player_id, 1);
    EXPECT_FLOAT_EQ(receivedState.players[0].state.x, 100.5f);
    EXPECT_EQ(receivedState.players[0].applied_upgrades.size(), 1);
    EXPECT_EQ(receivedState.race_finished, 0);
    EXPECT_TRUE(receivedState.final_results.empty());
}

// TEST 6: GameState Finalizado
TEST_F(ProtocolTest, GameStateFinished) {
    GameStateDTO state;
    state.car_count = 0;
    state.elapsed_time = 0.0f;
    state.race_finished = 1;

    PlayerResultDTO res1;
    res1.player_id = 10;
    res1.position = 1;
    res1.total_time = 120.0f;
    state.final_results.push_back(res1);

    PlayerResultDTO res2;
    res2.player_id = 20;
    res2.position = 2;
    res2.total_time = 130.0f;
    state.final_results.push_back(res2);

    serverProto->send_game_state(state);

    uint8_t cmd;
    clientProto->receive_command_code(cmd);
    
    GameStateDTO receivedState = clientProto->receive_game_state_payload();

    EXPECT_EQ(receivedState.race_finished, 1);
    ASSERT_EQ(receivedState.final_results.size(), 2);
    
    EXPECT_EQ(receivedState.final_results[0].player_id, 10);
    EXPECT_FLOAT_EQ(receivedState.final_results[0].total_time, 120.0f);
    
    EXPECT_EQ(receivedState.final_results[1].player_id, 20);
    EXPECT_FLOAT_EQ(receivedState.final_results[1].total_time, 130.0f);
}

// TEST 7: Refresh Match List
TEST_F(ProtocolTest, RefreshMatchList_RequestAndResponse) {
    clientProto->send_refresh_match_list();
    
    uint8_t cmd = serverProto->receiveCommand();
    ASSERT_EQ(cmd, CMD_REFRESH_MATCH_LIST);

    MatchListDTO listToSend;
    MatchInfo m1 = {.match_id = 0, .name = "Partida Rapida", .player_count = 4};
    MatchInfo m2 = {.match_id = 1, .name = "Solo Pros", .player_count = 1};
    listToSend.matches.push_back(m1);
    listToSend.matches.push_back(m2);

    serverProto->send_match_list(listToSend);

    uint8_t responseCmd;
    clientProto->receive_command_code(responseCmd);
    ASSERT_EQ(responseCmd, RSP_MATCH_LIST);

    MatchListDTO receivedList = clientProto->receive_match_list_payload();

    ASSERT_EQ(receivedList.matches.size(), 2);
    
    EXPECT_EQ(receivedList.matches[0].match_id, 0);
    EXPECT_EQ(receivedList.matches[0].name, "Partida Rapida"); 
    EXPECT_EQ(receivedList.matches[0].player_count, 4);
    
    EXPECT_EQ(receivedList.matches[1].match_id, 1);
    EXPECT_EQ(receivedList.matches[1].name, "Solo Pros");
    EXPECT_EQ(receivedList.matches[1].player_count, 1);
}


// TEST 8: Select Map (Cliente elige -> Server recibe)
TEST_F(ProtocolTest, SelectMap) {
    uint8_t mapIdToSend = 2;
    
    clientProto->send_select_map(mapIdToSend);

    uint8_t cmd = serverProto->receiveCommand();
    ASSERT_EQ(cmd, CMD_SELECT_MAP);

    uint8_t receivedMapId = serverProto->receiveUint8_t();
    
    EXPECT_EQ(receivedMapId, mapIdToSend);
}

// TEST 9: Select Car (Cliente elige -> Server recibe)
TEST_F(ProtocolTest, SelectCar) {
    uint8_t carIdToSend = 5;
    
    clientProto->send_select_car(carIdToSend);

    uint8_t cmd = serverProto->receiveCommand();
    ASSERT_EQ(cmd, CMD_SELECT_CAR);

    uint8_t receivedCarId = serverProto->receiveUint8_t();
    EXPECT_EQ(receivedCarId, carIdToSend);
}