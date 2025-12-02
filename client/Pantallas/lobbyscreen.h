#ifndef LOBBYSCREEN_H
#define LOBBYSCREEN_H

#include <QWidget>
#include <QTimer>
#include "client.h"
#include "../common/lobbyState.h"

namespace Ui {
class LobbyScreen;
}

struct ClientCarAttributes {
    float acceleration;
    float rotation_torque;
    float max_speed;
    uint8_t health;
};

struct CarDisplayInfo {
    QString name;
    QRect spriteRect;
    ClientCarAttributes stats;
};

class LobbyScreen : public QWidget {
    Q_OBJECT

public:
    explicit LobbyScreen(QWidget *parent = nullptr);
    ~LobbyScreen();
    void setClient(Client* client);
    std::map<uint8_t, QString> getPlayerNamesMap() const;
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
    int lastKnownMapId;

    bool wasIHost;
    bool isFirstUpdate;
    
    void updateBigMapPreview();
    void updateBigCarPreview();
    CarDisplayInfo getCarInfo(int carIndex);
    std::map<uint8_t, QString> namesCache;
};

#endif