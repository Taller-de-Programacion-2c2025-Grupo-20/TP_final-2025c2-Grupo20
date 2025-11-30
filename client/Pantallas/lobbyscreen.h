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
    void serverDisconnected();   

private slots:
    void updateLobbyState();
    void handleWaitingRoomState(const LobbyStateDTO& state);
    void handleSelectionState();
    void on_startButton_clicked();
    void on_joinButton_clicked();
    void on_createButton_clicked();
    void on_matchListWidget_currentItemChanged();

    void goToMapScreen();
    void goToCarScreen();
    void backToLobby();

    void onMapNext();
    void onMapPrev();
    void onCarNext();
    void onCarPrev();

private:
    Ui::LobbyScreen *ui;
    Client* client = nullptr;
    QTimer* poll_timer;
    QPixmap backgroundLobby;
    QPixmap backgroundMapSelect;
    QPixmap backgroundCarSelect;
    void paintEvent(QPaintEvent *event) override;

    int currentMapIndex = 0;
    int currentCarIndex = 0;
    int currentHostId = -1;
    
    const int TOTAL_MAPS = 3; 
    const int TOTAL_CARS = 7;

    void updateBigMapPreview();
    void updateBigCarPreview();
};

#endif