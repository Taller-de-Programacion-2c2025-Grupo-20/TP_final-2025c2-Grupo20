#include "lobbyscreen.h"
#include "ui_lobbyscreen.h"
#include "../common/constants.h"
#include "../common/clientCommand.h"
#include <QPushButton> 
#include <QListWidget>
#include <QLineEdit>
#include <QStackedWidget>
#include <QListWidgetItem>
#include <QDebug>

LobbyScreen::LobbyScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LobbyScreen) {
    
    ui->setupUi(this);
    
    poll_timer = new QTimer(this);
    connect(poll_timer, &QTimer::timeout, this, &LobbyScreen::updateLobbyState);

    connect(ui->createButton, &QPushButton::clicked, this, &LobbyScreen::on_createButton_clicked);
    connect(ui->joinButton, &QPushButton::clicked, this, &LobbyScreen::on_joinButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &LobbyScreen::on_startButton_clicked);
    connect(ui->matchListWidget, &QListWidget::currentItemChanged,
            this, &LobbyScreen::on_matchListWidget_currentItemChanged);

    ui->joinButton->setEnabled(false);
}

LobbyScreen::~LobbyScreen() {
    delete ui;
}

void LobbyScreen::setClient(Client* client) {
    this->client = client;
    poll_timer->start(250); 
    ui->startButton->setEnabled(false); 
    ui->stackedWidget_Lobby->setCurrentWidget(ui->page_Selection);
}

void LobbyScreen::updateLobbyState() {
    if (!client) return;
    MatchListDTO list = client->getReceiver().pollMatchList();
    
    QListWidgetItem* selected_item = ui->matchListWidget->currentItem();
    int selected_id = -1;
    if (selected_item) {
        selected_id = selected_item->data(Qt::UserRole).toInt();
    }
    
    ui->matchListWidget->clear();
    for (const auto& match : list.matches) {
        QString text = QString::fromStdString(match.name) + 
                       " (" + QString::number(match.player_count) + "/8)";
        
        QListWidgetItem* item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, (int)match.match_id); 
        ui->matchListWidget->addItem(item);

        if ((int)match.match_id == selected_id) {
            ui->matchListWidget->setCurrentItem(item);
        }
    }

    LobbyStateDTO state = client->getReceiver().pollLobbyState();
    
    if (!state.players.empty()) {
        if (ui->stackedWidget_Lobby->currentWidget() == ui->page_Selection) {
            ui->stackedWidget_Lobby->setCurrentWidget(ui->page_WaitingRoom);
        }

        ui->playerListWidget->clear();
        for (const auto& player : state.players) {
            ui->playerListWidget->addItem(QString::fromStdString(player.name));
        }

        if (client->getMyPlayerId() == state.host_id) {
            ui->startButton->setEnabled(true);
        }
    } else {
        ui->stackedWidget_Lobby->setCurrentWidget(ui->page_Selection);
    }
}

void LobbyScreen::on_createButton_clicked() {
    if (!client) return;
    std::string match_name = ui->matchNameEdit->text().toStdString();
    if (match_name.empty()) {
        return;
    }
    qDebug() << "CLIENT DEBUG 1: 'Create Match' clickeado. Nombre:" << QString::fromStdString(match_name);
    InputCmd cmd;
    cmd.player_id = client->getMyPlayerId();
    cmd.key = InputKey::CreateMatch;
    cmd.action = InputAction::Press;
    cmd.username = match_name; 
    
    client->push_input(cmd); 
}

void LobbyScreen::on_joinButton_clicked() {
    if (!client) return;

    QListWidgetItem* selected_item = ui->matchListWidget->currentItem();
    if (!selected_item) {
        return; 
    }

    uint8_t match_id_to_join = selected_item->data(Qt::UserRole).toInt();
    
    InputCmd cmd;
    cmd.player_id = client->getMyPlayerId();
    cmd.key = InputKey::JoinMatch;
    cmd.action = InputAction::Press;
    cmd.match_id = match_id_to_join;
    
    client->push_input(cmd); 

}

void LobbyScreen::on_startButton_clicked() {
    if (!client) return;
    
    InputCmd cmd;
    cmd.player_id = client->getMyPlayerId();
    cmd.key = InputKey::StartGame;
    cmd.action = InputAction::Press;
    
    client->push_input(cmd);
}

void LobbyScreen::on_matchListWidget_currentItemChanged() {
    bool is_item_selected = (ui->matchListWidget->currentItem() != nullptr);
    ui->joinButton->setEnabled(is_item_selected);
}