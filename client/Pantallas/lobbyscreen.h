#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#include <QWidget>
#include <QTimer>
#include "client.h"
#include "../common/lobbyState.h"

namespace Ui {
class LobbyScreen;
}

class LobbyScreen : public QWidget {
    Q_OBJECT

public:
    explicit LobbyScreen(QWidget *parent = nullptr);
    ~LobbyScreen();
    void setClient(Client* client);

signals:
    void startGame(Client* client);

private slots:
    void updateLobbyState();
    void on_readyButton_clicked();
    void on_startButton_clicked();

private:
    Ui::LobbyScreen *ui;
    Client* client = nullptr;
    QTimer* poll_timer;
};

#endif