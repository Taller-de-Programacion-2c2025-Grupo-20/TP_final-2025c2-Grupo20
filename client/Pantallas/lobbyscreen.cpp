#include "lobbyscreen.h"
#include "ui_lobbyscreen.h"
#include "../common/constants.h"
#include <QPushButton> 
#include <QListWidget>

LobbyScreen::LobbyScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LobbyScreen) {
    
    ui->setupUi(this);
    poll_timer = new QTimer(this);
    connect(poll_timer, &QTimer::timeout, this, &LobbyScreen::updateLobbyState);
    connect(ui->readyButton, &QPushButton::clicked, this, &LobbyScreen::on_readyButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &LobbyScreen::on_startButton_clicked);
}

LobbyScreen::~LobbyScreen() {
    delete ui;
}

void LobbyScreen::setClient(Client* client) {
    this->client = client;
    poll_timer->start(250);
    ui->startButton->setEnabled(false);
}

void LobbyScreen::updateLobbyState() {
    if (!client) return;

    LobbyStateDTO state = client->getReceiver().pollLobbyState();
    ui->playerListWidget->clear();
    for (const auto& player : state.players) {
        QString ready_str = player.is_ready ? "[LISTO]" : "[No Listo]";
        QString text = QString::fromStdString(player.name) + " " + ready_str;
        ui->playerListWidget->addItem(text);
    }

    if (client->getMyPlayerId() == state.host_id) {
        ui->startButton->setEnabled(true);
    }
}

void LobbyScreen::on_readyButton_clicked() {
    if (!client) return;
    
    InputCmd cmd;
    cmd.player_id = client->getMyPlayerId();
    cmd.key = InputKey::ToggleReady;
    cmd.action = InputAction::Press;
    
    client->push_input(cmd); 
}

void LobbyScreen::on_startButton_clicked() {
    // (Aquí iría la lógica para enviar un CMD_START_GAME
    InputCmd cmd;
    cmd.player_id = client->getMyPlayerId();
    cmd.key = InputKey::CreateMatch;
    cmd.action = InputAction::Press;
    client->push_input(cmd);
    poll_timer->stop();
    emit startGame(client);
}