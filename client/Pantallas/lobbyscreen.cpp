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
#include <QStyleOption>
#include <QPainter>
#include <QStyle>

LobbyScreen::LobbyScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LobbyScreen) {
    
    ui->setupUi(this);
    backgroundLobby.load(":/Pantallas/fondo_lobby.png");
    backgroundMapSelect.load(":/Pantallas/fondo_select_map.png");
    backgroundCarSelect.load(":/Pantallas/fondo_garage.png");

    ui->stackedWidget_Lobby->setStyleSheet("background: transparent;");
    ui->page_WaitingRoom->setStyleSheet("background: transparent;");
    ui->page_Selection->setStyleSheet("background: transparent;");
    ui->page_MapSelect->setStyleSheet("background: transparent;");
    ui->page_CarSelect->setStyleSheet("background: transparent;");

    poll_timer = new QTimer(this);
    connect(poll_timer, &QTimer::timeout, this, &LobbyScreen::updateLobbyState);

    connect(ui->createButton, &QPushButton::clicked, this, &LobbyScreen::on_createButton_clicked);
    connect(ui->joinButton, &QPushButton::clicked, this, &LobbyScreen::on_joinButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &LobbyScreen::on_startButton_clicked);
    connect(ui->matchListWidget, &QListWidget::currentItemChanged,
            this, &LobbyScreen::on_matchListWidget_currentItemChanged);
    
    connect(ui->btnGoToMap, &QPushButton::clicked, this, &LobbyScreen::goToMapScreen);
    connect(ui->btnGoToCar, &QPushButton::clicked, this, &LobbyScreen::goToCarScreen);
    
    connect(ui->btnMapSelect, &QPushButton::clicked, this, &LobbyScreen::backToLobby);
    connect(ui->btnCarSelect, &QPushButton::clicked, this, &LobbyScreen::backToLobby);

    connect(ui->btnMapNext, &QPushButton::clicked, this, &LobbyScreen::onMapNext);
    connect(ui->btnMapPrev, &QPushButton::clicked, this, &LobbyScreen::onMapPrev);

    connect(ui->btnCarNext, &QPushButton::clicked, this, &LobbyScreen::onCarNext);
    connect(ui->btnCarPrev, &QPushButton::clicked, this, &LobbyScreen::onCarPrev);
    
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

    if (client->getReceiver().isServerDown()) {
        poll_timer->stop();

        emit serverDisconnected();
        return;
    }

    LobbyStateDTO state = client->getReceiver().pollLobbyState();

    if (!state.players.empty()) {
        handleWaitingRoomState(state);
    } else {
        handleSelectionState();
    }
}


void LobbyScreen::handleSelectionState() {
    if (ui->stackedWidget_Lobby->currentWidget() != ui->page_Selection) {
        ui->stackedWidget_Lobby->setCurrentWidget(ui->page_Selection);
    }
    client->send_refresh_request(); 
    MatchListDTO list = client->getReceiver().pollMatchList();
    int selected_id = -1;
    if (QListWidgetItem* current = ui->matchListWidget->currentItem()) {
        selected_id = current->data(Qt::UserRole).toInt();
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
}

void LobbyScreen::on_createButton_clicked() {
    if (!client) return;
    std::string match_name = ui->matchNameEdit->text().toStdString();
    if (match_name.empty()) {
        return;
    }
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

void LobbyScreen::paintEvent(QPaintEvent *) {
    QPainter p(this);
    QWidget* currentWidget = ui->stackedWidget_Lobby->currentWidget();
    QPixmap* bgToDraw = &backgroundLobby; 

    if (currentWidget == ui->page_MapSelect) {
        bgToDraw = &backgroundMapSelect;
    } 
    else if (currentWidget == ui->page_CarSelect) {
        bgToDraw = &backgroundCarSelect;
    }

    if (!bgToDraw->isNull()) {
        p.drawPixmap(rect(), *bgToDraw);
    }
}


void LobbyScreen::goToMapScreen() {
    bool amIHost = (client && client->getMyPlayerId() == currentHostId);
    ui->btnMapPrev->setVisible(amIHost);
    ui->btnMapNext->setVisible(amIHost);
    if (!amIHost) {
        ui->btnMapSelect->setText("Volver al Lobby");
    }

    ui->stackedWidget_Lobby->setCurrentWidget(ui->page_MapSelect);
    updateBigMapPreview();
    this->update();
}

void LobbyScreen::goToCarScreen() {
    ui->stackedWidget_Lobby->setCurrentWidget(ui->page_CarSelect);
    updateBigCarPreview();
    this->update();
}

void LobbyScreen::backToLobby() {
    bool amIHost = (client && client->getMyPlayerId() == currentHostId);

    if (client) {
        if (ui->stackedWidget_Lobby->currentWidget() == ui->page_MapSelect) {
             if (amIHost) {
                 InputCmd cmd;
                 cmd.player_id = client->getMyPlayerId();
                 cmd.key = InputKey::SelectMap; 
                 cmd.action = InputAction::Press;
                 cmd.match_id = static_cast<uint8_t>(currentMapIndex);
                 client->push_input(cmd);
             }
        }
        else if (ui->stackedWidget_Lobby->currentWidget() == ui->page_CarSelect) {
             InputCmd cmd;
             cmd.player_id = client->getMyPlayerId();
             cmd.key = InputKey::SelectCar; 
             cmd.action = InputAction::Press;
             cmd.match_id = static_cast<uint8_t>(currentCarIndex);
             client->push_input(cmd);
        }
    }
    ui->stackedWidget_Lobby->setCurrentWidget(ui->page_WaitingRoom);
    this->update();
}


void LobbyScreen::onMapNext() {
    currentMapIndex = (currentMapIndex + 1) % TOTAL_MAPS;
    updateBigMapPreview();
    
    if (client && client->getMyPlayerId() == currentHostId) {
         InputCmd cmd;
         cmd.player_id = client->getMyPlayerId();
         cmd.key = InputKey::SelectMap; 
         cmd.action = InputAction::Press;
         cmd.match_id = static_cast<uint8_t>(currentMapIndex);
         client->push_input(cmd);
    }
}

void LobbyScreen::onMapPrev() {
    currentMapIndex = (currentMapIndex - 1 + TOTAL_MAPS) % TOTAL_MAPS;
    updateBigMapPreview();

    if (client && client->getMyPlayerId() == currentHostId) {
         InputCmd cmd;
         cmd.player_id = client->getMyPlayerId();
         cmd.key = InputKey::SelectMap; 
         cmd.action = InputAction::Press;
         cmd.match_id = static_cast<uint8_t>(currentMapIndex);
         client->push_input(cmd);
    }
}

void LobbyScreen::onCarNext() {
    currentCarIndex = (currentCarIndex + 1) % TOTAL_CARS;
    updateBigCarPreview();
}

void LobbyScreen::onCarPrev() {
    currentCarIndex = (currentCarIndex - 1 + TOTAL_CARS) % TOTAL_CARS;
    updateBigCarPreview();
}


void LobbyScreen::updateBigMapPreview() {
    QString filename;
    QString mapName;

    switch (currentMapIndex) {
        case 0: 
            filename = LIBERTY_CITY_FILE;
            mapName = LIBERTY; 
            break;
        case 1:
            filename = VICE_CITY_FILE;
            mapName = VICE;
            break;
        case 2: 
            filename = SAN_ANDREAS_FILE;
            mapName = ANDREAS;
            break;
        default:
            return;
    }

    ui->label_MapName->setText(mapName);
    QString fullPath = ":/data/cities/" + filename;
    QPixmap pix(fullPath);
    
    if (!pix.isNull()) {
        ui->label_BigMapPreview->setPixmap(pix.scaled(ui->label_BigMapPreview->size(), 
                                                      Qt::KeepAspectRatio, 
                                                      Qt::SmoothTransformation));
    }
}

void LobbyScreen::updateBigCarPreview() {
    QPixmap spritesheet(":/data/cars/Mobile - Grand Theft Auto 4 - Miscellaneous - Cars.png"); 

    int x = 0, y = 0, w = 0, h = 0;
    CarType tipo = static_cast<CarType>(currentCarIndex); 

    switch (tipo) {
        case CarType::VERDE:        x = 0; y = 0;   w = 32; h = 32; break;
        case CarType::ROJO:         x = 0; y = 64;  w = 40; h = 40; break;
        case CarType::DESCAPOTABLE: x = 0; y = 144; w = 40; h = 40; break;
        case CarType::CELESTE:      x = 0; y = 224; w = 40; h = 40; break;
        case CarType::JEEP:         x = 0; y = 304; w = 40; h = 40; break;
        case CarType::CAMIONETA:    x = 0; y = 384; w = 40; h = 40; break;
        case CarType::CAMION:       x = 0; y = 464; w = 48; h = 48; break;
        default:
            ui->label_BigCarPreview->clear();
            return;
    }
    QPixmap carSprite = spritesheet.copy(x, y, w, h);
    QPixmap scaledSprite = carSprite.scaled(ui->label_BigCarPreview->size(), 
                                            Qt::KeepAspectRatio, 
                                            Qt::FastTransformation);

    ui->label_BigCarPreview->setPixmap(scaledSprite);
    ui->label_BigCarPreview->setAlignment(Qt::AlignCenter); 
}

void LobbyScreen::handleWaitingRoomState(const LobbyStateDTO& state) {
    currentHostId = state.host_id;

    bool amIHost = (client->getMyPlayerId() == state.host_id);
    bool inMapScreen = (ui->stackedWidget_Lobby->currentWidget() == ui->page_MapSelect);

    if (!amIHost || !inMapScreen) {
        currentMapIndex = state.map_id;
    }

    QWidget* current = ui->stackedWidget_Lobby->currentWidget();
    if (current != ui->page_WaitingRoom && 
        current != ui->page_MapSelect && 
        current != ui->page_CarSelect) {
        ui->stackedWidget_Lobby->setCurrentWidget(ui->page_WaitingRoom);
    }

    ui->playerListWidget->clear();
    for (const auto& player : state.players) {
        QString carName;
        switch (static_cast<CarType>(player.car_id)) {
            case CarType::VERDE:        carName = VERDE; break;
            case CarType::ROJO:         carName = ROJO; break;
            case CarType::DESCAPOTABLE: carName = DESCAPOTABLE; break;
            case CarType::CELESTE:      carName = CELESTE; break;
            case CarType::JEEP:         carName = JEEP; break;
            case CarType::CAMIONETA:    carName = CAMIONETA; break;
            case CarType::CAMION:       carName = CAMION; break;
            default: break;
        }
        QString text = QString::fromStdString(player.name) + " (" + carName + ")";
        ui->playerListWidget->addItem(text);
    }

    if (client->getMyPlayerId() == state.host_id) {
        ui->startButton->setEnabled(true);
        ui->btnGoToMap->setEnabled(true);
    } else {
        ui->startButton->setEnabled(false);
        ui->startButton->setText("Esperando \n\n al host...");

        ui->btnGoToMap->setEnabled(true); 
        ui->btnGoToMap->setText("Ver mapa");
    }

    ui->btnGoToCar->setEnabled(true);
    if (inMapScreen) {
        updateBigMapPreview();
        this->update(); 
    }
}