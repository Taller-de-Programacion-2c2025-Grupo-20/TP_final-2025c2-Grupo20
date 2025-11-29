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
    this->setObjectName("LobbyScreen");

    this->setStyleSheet(
        "#LobbyScreen { "
        "   border-image: url(:/Pantallas/fondo_lobby.png) 0 0 0 0 stretch stretch;"
        "   font-family: 'Press Start 2P';"
        "}"
    );

    setupComboBoxes();
    poll_timer = new QTimer(this);
    connect(poll_timer, &QTimer::timeout, this, &LobbyScreen::updateLobbyState);

    connect(ui->createButton, &QPushButton::clicked, this, &LobbyScreen::on_createButton_clicked);
    connect(ui->joinButton, &QPushButton::clicked, this, &LobbyScreen::on_joinButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &LobbyScreen::on_startButton_clicked);
    connect(ui->matchListWidget, &QListWidget::currentItemChanged,
            this, &LobbyScreen::on_matchListWidget_currentItemChanged);
    
    connect(ui->mapComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LobbyScreen::on_mapComboBox_currentIndexChanged);

    connect(ui->carComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &LobbyScreen::on_carComboBox_currentIndexChanged);

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

void LobbyScreen::handleWaitingRoomState(const LobbyStateDTO& state) {
    if (ui->stackedWidget_Lobby->currentWidget() != ui->page_WaitingRoom) {
        ui->stackedWidget_Lobby->setCurrentWidget(ui->page_WaitingRoom);
    }
    ui->playerListWidget->clear();
    for (const auto& player : state.players) {
        QString carName = ui->carComboBox->itemText(player.car_id);
        QString text = QString::fromStdString(player.name) + " (" + carName + ")";
        ui->playerListWidget->addItem(text);
    }

    if (client->getMyPlayerId() == state.host_id) {
        ui->startButton->setEnabled(true);
        ui->mapComboBox->setEnabled(true);
        ui->startButton->setText("Iniciar Partida");
    } else {
        ui->startButton->setEnabled(false);
        ui->mapComboBox->setEnabled(false);
        ui->startButton->setText("Esperando al Host...");
        if (ui->mapComboBox->currentIndex() != state.map_id) {
            const QSignalBlocker blocker(ui->mapComboBox);
            ui->mapComboBox->setCurrentIndex(state.map_id);
            showMapPreview(state.map_id);
        }
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

void LobbyScreen::setupComboBoxes() {
    ui->mapComboBox->clear();
    ui->mapComboBox->addItem(LIBERTY, 0); 
    ui->mapComboBox->addItem(VICE, 1);    
    ui->mapComboBox->addItem(ANDREAS, 2);  

    ui->carComboBox->clear();
    ui->carComboBox->addItem(VERDE, 0);        
    ui->carComboBox->addItem(ROJO, 1);         
    ui->carComboBox->addItem(DESCAPOTABLE, 2); 
    ui->carComboBox->addItem(CELESTE, 3);    
    ui->carComboBox->addItem(JEEP, 4);         
    ui->carComboBox->addItem(CAMIONETA, 5);    
    ui->carComboBox->addItem(CAMION, 6);      

    showMapPreview(0);
    showCarPreview(0);
}

void LobbyScreen::showCarPreview(int index) {
    QPixmap spritesheet(":/data/cars/Mobile - Grand Theft Auto 4 - Miscellaneous - Cars.png"); 

    if (spritesheet.isNull()) {
        ui->label_carPreview->setText("Img no encontrada");
        return;
    }

    int x = 0, y = 0, w = 0, h = 0;
    CarType tipo = static_cast<CarType>(index);
    switch (tipo) {
        case CarType::VERDE: x = 0; y = 0; w = 32; h = 32; break;
        case CarType::ROJO:  x = 0; y = 64; w = 40; h = 40; break;
        case CarType::DESCAPOTABLE: x = 0; y = 144; w = 40; h = 40; break;
        case CarType::CELESTE: x = 0; y = 224; w = 40; h = 40; break;
        case CarType::JEEP: x = 0; y = 304; w = 40; h = 40; break;
        case CarType::CAMIONETA: x = 0; y = 384; w = 40; h = 40; break;
        case CarType::CAMION: x = 0; y = 464; w = 48; h = 48; break;
        default:
            ui->label_carPreview->clear();
            return;
    }
    QPixmap carSprite = spritesheet.copy(x, y, w, h);

    int previewSize = 120; 
    QPixmap scaledSprite = carSprite.scaled(previewSize, previewSize, 
                                            Qt::KeepAspectRatio, 
                                            Qt::FastTransformation);

    ui->label_carPreview->setPixmap(scaledSprite);
}

void LobbyScreen::showMapPreview(int index) {
    QString filename;
    switch (index) {
        case 0: 
            filename = LIBERTY_CITY_FILE;
            break;
        case 1:
            filename = VICE_CITY_FILE;
            break;
        case 2: 
            filename = SAN_ANDREAS_FILE;
            break;
        default:
            ui->label_mapPreview->clear();
            return;
    }
    QString fullPath = ":/data/cities/" + filename;
    QPixmap pix(fullPath);
    
    if (!pix.isNull()) {
        ui->label_mapPreview->setPixmap(pix.scaled(ui->label_mapPreview->size(), 
                                                   Qt::KeepAspectRatio, 
                                                   Qt::SmoothTransformation));
    } else {
        qDebug() << "ERROR: No se cargó el mapa. Ruta:" << fullPath;
        ui->label_mapPreview->setText("Sin Preview");
    }
}


void LobbyScreen::on_mapComboBox_currentIndexChanged(int index) {
    showMapPreview(index);
    if (client && ui->mapComboBox->isEnabled()) {
        
        InputCmd cmd;
        cmd.player_id = client->getMyPlayerId();
        cmd.key = InputKey::SelectMap; 
        cmd.action = InputAction::Press;
        cmd.match_id = static_cast<uint8_t>(index); 
        
        client->push_input(cmd);
    }
}

void LobbyScreen::on_carComboBox_currentIndexChanged(int index) {
    showCarPreview(index);
    if (client) {
        
        InputCmd cmd;
        cmd.player_id = client->getMyPlayerId();
        cmd.key = InputKey::SelectCar; 
        cmd.action = InputAction::Press;
        cmd.match_id = static_cast<uint8_t>(index); 
        
        client->push_input(cmd);
    }
}

void LobbyScreen::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
