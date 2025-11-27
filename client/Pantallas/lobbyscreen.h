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
    void handleWaitingRoomState(const LobbyStateDTO& state);
    void handleSelectionState();
    void on_startButton_clicked();
    void on_joinButton_clicked();
    void on_createButton_clicked();
    void on_matchListWidget_currentItemChanged();
    void setupComboBoxes();
    void showMapPreview(int index);
    void showCarPreview(int index);
    void on_mapComboBox_currentIndexChanged(int index);
    void on_carComboBox_currentIndexChanged(int index);

private:
    Ui::LobbyScreen *ui;
    Client* client = nullptr;
    QTimer* poll_timer;
    void paintEvent(QPaintEvent *event) override;
};

#endif