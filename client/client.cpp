#include <memory>
#include <QApplication>
#include "mainwindow.h"
#include "game_window.h"
#include "client.h"

Client::Client(const char* host, const char* port)
    : protocol(host, port), my_player_id(0) {}

bool Client::try_login(const std::string& username) {
    protocol.send_login_attempt(username);
    uint8_t code = protocol.receive_login_response(my_player_id);
    return (code == CMD_LOGIN);
}

int Client::runGame() {
    GameWindow gw(*this);
    return gw.runGame();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::unique_ptr<Client> client_for_game;
    MainWindow w(nullptr);

    QObject::connect(&w, &MainWindow::loginOk, [&](Client* readyClient) {
        client_for_game.reset(readyClient);
        app.quit();
    });

    w.show();
    app.exec();

    if (client_for_game) return client_for_game->runGame();
    return EXIT_SUCCESS;
}