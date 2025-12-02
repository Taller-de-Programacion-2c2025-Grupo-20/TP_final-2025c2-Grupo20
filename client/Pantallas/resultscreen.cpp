#include "resultscreen.h"
#include "ui_resultscreen.h"
#include <QTableWidgetItem>

ResultScreen::ResultScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultScreen) {
    ui->setupUi(this);

    // Configurar Tabla
    ui->table_Ranking->setColumnCount(3);
    ui->table_Ranking->setHorizontalHeaderLabels({"POS", "JUGADOR", "TIEMPO"});
    ui->table_Ranking->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_Ranking->verticalHeader()->setVisible(false);
    ui->table_Ranking->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_Ranking->setSelectionMode(QAbstractItemView::NoSelection);
    
    // Estilo Retro para la tabla
    ui->table_Ranking->setStyleSheet(
        "QTableWidget { background-color: transparent; color: white; font-family: 'Press Start 2P'; font-size: 12px; gridline-color: white; }"
        "QHeaderView::section { background-color: #032766; color: white; font-family: 'Press Start 2P'; }"
    );
}

ResultScreen::~ResultScreen() {
    delete ui;
}

void ResultScreen::setResults(const std::vector<PlayerResultDTO>& results, 
                              const std::map<uint8_t, QString>& idToNameMap,
                              uint8_t myPlayerId) {
    
    ui->table_Ranking->setRowCount(results.size());

    bool iWon = false;

    for (int i = 0; i < static_cast<int>(results.size()); ++i) {
        const auto& res = results[i];

        // 1. Obtener Nombre
        QString name = "Jugador " + QString::number(res.player_id);
        if (idToNameMap.count(res.player_id)) {
            name = idToNameMap.at(res.player_id);
        }

        // 2. Determinar si gané (Posición 1 y soy yo)
        if (res.player_id == myPlayerId && res.position == 1) {
            iWon = true;
        }

        // 3. Llenar Tabla
        QTableWidgetItem* itemPos = new QTableWidgetItem(QString::number(res.position));
        QTableWidgetItem* itemName = new QTableWidgetItem(name);
        QTableWidgetItem* itemTime = new QTableWidgetItem(QString::number(res.total_time, 'f', 2) + "s");

        // Centrar textos
        itemPos->setTextAlignment(Qt::AlignCenter);
        itemName->setTextAlignment(Qt::AlignCenter);
        itemTime->setTextAlignment(Qt::AlignCenter);

        // Resaltar mi fila
        if (res.player_id == myPlayerId) {
            itemPos->setBackground(QColor(255, 255, 0, 50)); // Amarillo transparente
            itemName->setBackground(QColor(255, 255, 0, 50));
            itemTime->setBackground(QColor(255, 255, 0, 50));
        }

        ui->table_Ranking->setItem(i, 0, itemPos);
        ui->table_Ranking->setItem(i, 1, itemName);
        ui->table_Ranking->setItem(i, 2, itemTime);
    }

    // 4. Mensaje Principal
    if (iWon) {
        ui->label_Outcome->setText("¡VICTORIA!");
        ui->label_Outcome->setStyleSheet("color: #00FF00; font-family: 'Press Start 2P'; font-size: 24pt; font-weight: bold;");
    } else {
        ui->label_Outcome->setText("FIN DE PARTIDA");
        ui->label_Outcome->setStyleSheet("color: #FF0000; font-family: 'Press Start 2P'; font-size: 24pt; font-weight: bold;");
    }
}

void ResultScreen::on_btnExit_clicked() {
    QApplication::quit();
}

void ResultScreen::paintEvent(QPaintEvent *) {
    QPainter p(this);
    if (!backgroundImage.isNull()) {
        p.drawPixmap(rect(), backgroundImage);
    } else {
        p.fillRect(rect(), Qt::black);
    }
}