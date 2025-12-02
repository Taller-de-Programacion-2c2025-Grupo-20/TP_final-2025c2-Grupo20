#include "lobbyscreen.h"

#include <QDebug>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPainter>
#include <QPushButton>
#include <QStackedWidget>
#include <QStyle>
#include <QStyleOption>

#include "../common/clientCommand.h"
#include "../common/constants.h"

#include "ui_lobbyscreen.h"

LobbyScreen::LobbyScreen(QWidget* parent): QWidget(parent), ui(new Ui::LobbyScreen) {

    ui->setupUi(this);
    backgroundLobby.load(LOBBY_FILE);
    backgroundMapSelect.load(SELECT_MAP_FILE);
    backgroundCarSelect.load(GARAGE_FILE);

    ui->stackedWidget_Lobby->setStyleSheet(BACKGROUND_STYLE);
    ui->page_WaitingRoom->setStyleSheet(BACKGROUND_STYLE);
    ui->label_Notification->setVisible(false);
    ui->page_Selection->setStyleSheet(BACKGROUND_STYLE);
    ui->page_MapSelect->setStyleSheet(BACKGROUND_STYLE);
    ui->page_CarSelect->setStyleSheet(BACKGROUND_STYLE);

    ui->bar_Speed->setRange(0, 14);
    ui->bar_Speed->setTextVisible(false);
    ui->bar_Accel->setRange(0, 60);
    ui->bar_Accel->setTextVisible(false);
    ui->bar_Sensitivity->setRange(0, 100);
    ui->bar_Sensitivity->setTextVisible(false);
    ui->bar_Health->setRange(0, 160);
    ui->bar_Health->setTextVisible(false);

    poll_timer = new QTimer(this);
    connect(poll_timer, &QTimer::timeout, this, &LobbyScreen::updateLobbyState);

    connect(ui->createButton, &QPushButton::clicked, this, &LobbyScreen::on_createButton_clicked);
    connect(ui->joinButton, &QPushButton::clicked, this, &LobbyScreen::on_joinButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &LobbyScreen::on_startButton_clicked);
    connect(ui->matchListWidget, &QListWidget::currentItemChanged, this,
            &LobbyScreen::on_matchListWidget_currentItemChanged);

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

LobbyScreen::~LobbyScreen() { delete ui; }

void LobbyScreen::setClient(Client* client) {
    this->client = client;
    this->lastKnownMapId = -1;

    this->wasIHost = false;
    this->isFirstUpdate = true;

    poll_timer->start(250);
    ui->startButton->setEnabled(false);
    ui->stackedWidget_Lobby->setCurrentWidget(ui->page_Selection);
}

void LobbyScreen::updateLobbyState() {
    if (!client)
        return;

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
    for (const auto& match: list.matches) {
        QString text = QString::fromStdString(match.name) + " (" +
                       QString::number(match.player_count) + "/8)";

        QListWidgetItem* item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, (int)match.match_id);
        ui->matchListWidget->addItem(item);
        if ((int)match.match_id == selected_id) {
            ui->matchListWidget->setCurrentItem(item);
        }
    }
}

void LobbyScreen::on_createButton_clicked() {
    if (!client)
        return;

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
    if (!client)
        return;

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
    if (!client)
        return;

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

void LobbyScreen::paintEvent(QPaintEvent*) {
    QPainter p(this);
    QWidget* currentWidget = ui->stackedWidget_Lobby->currentWidget();
    QPixmap* bgToDraw = &backgroundLobby;

    if (currentWidget == ui->page_MapSelect) {
        bgToDraw = &backgroundMapSelect;
    } else if (currentWidget == ui->page_CarSelect) {
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
        } else if (ui->stackedWidget_Lobby->currentWidget() == ui->page_CarSelect) {
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
    QString fullPath = ":/data/assets/" + filename;
    QPixmap pix(fullPath);

    if (!pix.isNull()) {
        ui->label_BigMapPreview->setPixmap(pix.scaled(
                ui->label_BigMapPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void LobbyScreen::updateBigCarPreview() {
    CarDisplayInfo info = getCarInfo(currentCarIndex);

    if (ui->label_CarName) {
        ui->label_CarName->setText(info.name);
    }

    QPixmap spritesheet(":/data/cars/" + QString(CARS_FILE));
    if (!spritesheet.isNull()) {
        QPixmap carSprite = spritesheet.copy(info.spriteRect);

        const QColor keyColor(163, 163, 13);

        QImage img = carSprite.toImage().convertToFormat(QImage::Format_ARGB32);

        for (int y = 0; y < img.height(); ++y) {
            QRgb* row = reinterpret_cast<QRgb*>(img.scanLine(y));
            for (int x = 0; x < img.width(); ++x) {
                QColor c = QColor::fromRgba(row[x]);
                if (c.red() == keyColor.red() && c.green() == keyColor.green() &&
                    c.blue() == keyColor.blue()) {
                    c.setAlpha(0);
                    row[x] = c.rgba();
                }
            }
        }

        carSprite = QPixmap::fromImage(img);

        QPixmap scaledSprite = carSprite.scaled(ui->label_BigCarPreview->size(),
                                                Qt::KeepAspectRatio, Qt::FastTransformation);

        ui->label_BigCarPreview->setPixmap(scaledSprite);
        ui->label_BigCarPreview->setAlignment(Qt::AlignCenter);
        ui->label_BigCarPreview->setScaledContents(false);
        ui->label_BigCarPreview->setStyleSheet("background: transparent;");
    }

    ui->bar_Speed->setValue(static_cast<int>(info.stats.max_speed));
    ui->bar_Accel->setValue(static_cast<int>(info.stats.acceleration));
    float calculatedSens = (maxSens - info.stats.rotation_torque);
    ui->bar_Sensitivity->setValue(static_cast<int>(calculatedSens * 20));
    ui->bar_Health->setValue(static_cast<int>(info.stats.health));
}

void LobbyScreen::handleWaitingRoomState(const LobbyStateDTO& state) {
    currentHostId = state.host_id;
    bool amIHost = (client->getMyPlayerId() == currentHostId);

    QWidget* current = ui->stackedWidget_Lobby->currentWidget();
    if (current != ui->page_WaitingRoom && current != ui->page_MapSelect &&
        current != ui->page_CarSelect) {
        ui->stackedWidget_Lobby->setCurrentWidget(ui->page_WaitingRoom);
    }

    bool inMapScreen = (current == ui->page_MapSelect);
    if (!amIHost || !inMapScreen) {
        currentMapIndex = state.map_id;
    }

    if (!isFirstUpdate && !wasIHost && amIHost) {
        ui->label_Notification->setText("¡Ahora eres el host!");
        ui->label_Notification->show();
        ui->label_Notification->adjustSize();

        int parentWidth = ui->page_WaitingRoom->width();
        ui->label_Notification->move((parentWidth - ui->label_Notification->width()) / 2, 10);

        QTimer::singleShot(4000, ui->label_Notification, &QLabel::hide);
    }

    wasIHost = amIHost;
    isFirstUpdate = false;
    if (amIHost) {
        ui->startButton->setEnabled(true);
        ui->startButton->setText("INICIAR PARTIDA");
        ui->btnGoToMap->setText("SELECCIONAR \n\n MAPA");
    } else {
        ui->startButton->setEnabled(false);
        ui->startButton->setText("Esperando \n\n al host...");
        ui->btnGoToMap->setText("VER MAPA");
    }

    ui->btnGoToMap->setEnabled(true);
    ui->btnGoToCar->setEnabled(true);
    namesCache.clear();
    ui->playerListWidget->clear();
    for (const auto& player: state.players) {
        namesCache[player.player_id] = QString::fromStdString(player.name);
        CarDisplayInfo info = getCarInfo(player.car_id);
        QString text = QString::fromStdString(player.name) + " (" + info.name + ")";
        ui->playerListWidget->addItem(text);
    }

    if (inMapScreen) {
        ui->btnMapPrev->setVisible(amIHost);
        ui->btnMapNext->setVisible(amIHost);

        if (amIHost) {
            ui->btnMapSelect->setText("SELECCIONAR");
        } else {
            ui->btnMapSelect->setText("VOLVER");
        }

        updateBigMapPreview();
        this->update();
    }
}

CarDisplayInfo LobbyScreen::getCarInfo(int carIndex) {
    CarType type = static_cast<CarType>(carIndex);
    QRect rect(0, 0, 0, 0);
    ClientCarAttributes stats = {0, 0, 0, 0};
    QString name;

    switch (type) {
        case CarType::VERDE:
            name = VERDE;
            rect = QRect(0, 0, 32, 32);
            stats = {52.f, 10.5f, 11.f, 100};
            break;
        case CarType::ROJO:
            name = ROJO;
            rect = QRect(0, 64, 40, 40);
            stats = {60.f, 9.5f, 14.f, 90};
            break;
        case CarType::DESCAPOTABLE:
            name = DESCAPOTABLE;
            rect = QRect(0, 144, 40, 40);
            stats = {58.f, 11.f, 13.f, 85};
            break;
        case CarType::CELESTE:
            name = CELESTE;
            rect = QRect(0, 224, 40, 40);
            stats = {54.f, 10.5f, 12.f, 105};
            break;
        case CarType::JEEP:
            name = JEEP;
            rect = QRect(0, 304, 40, 40);
            stats = {50.f, 11.5f, 11.f, 120};
            break;
        case CarType::CAMIONETA:
            name = CAMIONETA;
            rect = QRect(0, 384, 40, 40);
            stats = {48.f, 12.f, 10.5f, 130};
            break;
        case CarType::CAMION:
            name = CAMION;
            rect = QRect(0, 464, 48, 48);
            stats = {38.f, 13.5f, 8.5f, 160};
            break;
    }

    return {name, rect, stats};
}

std::map<uint8_t, QString> LobbyScreen::getPlayerNamesMap() const { return this->namesCache; }