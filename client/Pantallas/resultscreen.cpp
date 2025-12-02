#include "resultscreen.h"

#include <QTableWidgetItem>

#include "ui_resultscreen.h"

ResultScreen::ResultScreen(QWidget* parent): QWidget(parent), ui(new Ui::ResultScreen) {
    ui->setupUi(this);

    backgroundImage.load(QString::fromStdString(RESULT_FILE));
    ui->table_Ranking->setColumnCount(3);
    ui->table_Ranking->setHorizontalHeaderLabels({"POS", "JUGADOR", "TIEMPO"});
    ui->table_Ranking->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_Ranking->verticalHeader()->setVisible(false);
    ui->table_Ranking->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_Ranking->setSelectionMode(QAbstractItemView::NoSelection);
}

ResultScreen::~ResultScreen() { delete ui; }

void ResultScreen::setResults(const std::vector<PlayerResultDTO>& results,
                              const std::map<uint8_t, QString>& idToNameMap, uint8_t myPlayerId) {

    int ROW_HEIGHT = 40;
    int HEADER_HEIGHT = 45;
    int BORDER_WIDTH = 6;

    ui->table_Ranking->setRowCount(8);
    ui->table_Ranking->verticalHeader()->setDefaultSectionSize(ROW_HEIGHT);
    ui->table_Ranking->horizontalHeader()->setFixedHeight(HEADER_HEIGHT);

    int totalHeight = HEADER_HEIGHT + (8 * ROW_HEIGHT) + (2 * BORDER_WIDTH);
    ui->table_Ranking->setFixedHeight(totalHeight);


    bool iWon = false;

    for (int i = 0; i < 8; ++i) {

        if (i >= static_cast<int>(results.size())) {
            QTableWidgetItem* itemPos = new QTableWidgetItem("");
            QTableWidgetItem* itemName = new QTableWidgetItem("- VACÍO -");
            QTableWidgetItem* itemTime = new QTableWidgetItem("--:--");

            itemName->setTextAlignment(Qt::AlignCenter);
            itemTime->setTextAlignment(Qt::AlignCenter);

            ui->table_Ranking->setItem(i, 0, itemPos);
            ui->table_Ranking->setItem(i, 1, itemName);
            ui->table_Ranking->setItem(i, 2, itemTime);
            continue;
        }

        const auto& res = results[i];

        QString name = "Jugador " + QString::number(res.player_id);
        if (idToNameMap.count(res.player_id)) {
            name = idToNameMap.at(res.player_id);
        }

        if (res.player_id == myPlayerId && res.position == 1)
            iWon = true;

        QTableWidgetItem* itemPos = new QTableWidgetItem(QString::number(res.position));
        QTableWidgetItem* itemName = new QTableWidgetItem(name);
        QTableWidgetItem* itemTime =
                new QTableWidgetItem(QString::number(res.total_time, 'f', 2) + "s");

        itemPos->setTextAlignment(Qt::AlignCenter);
        itemName->setTextAlignment(Qt::AlignCenter);
        itemTime->setTextAlignment(Qt::AlignCenter);

        QColor textColor;
        QFont font = itemPos->font();

        if (res.position == 1) {
            textColor = QColor("#FFCC00");
        } else {
            textColor = QColor("#CCCCCC");
        }

        itemPos->setForeground(textColor);
        itemName->setForeground(textColor);
        itemTime->setForeground(textColor);

        if (res.player_id == myPlayerId) {
            QColor highlight(255, 255, 255, 30);
            itemPos->setBackground(highlight);
            itemName->setBackground(highlight);
            itemTime->setBackground(highlight);
        }

        ui->table_Ranking->setItem(i, 0, itemPos);
        ui->table_Ranking->setItem(i, 1, itemName);
        ui->table_Ranking->setItem(i, 2, itemTime);
    }

    if (iWon) {
        ui->label_Outcome->setText("¡VICTORIA!");
        ui->label_Outcome->setStyleSheet("color: #00FF00; font-family: 'Press Start 2P'; "
                                         "font-size: 24pt; font-weight: bold;");
    } else {
        ui->label_Outcome->setText("HAS PERDIDO :(");
        ui->label_Outcome->setStyleSheet("color: #FF0000; font-family: 'Press Start 2P'; "
                                         "font-size: 24pt; font-weight: bold;");
    }
}

void ResultScreen::paintEvent(QPaintEvent*) {
    QPainter p(this);
    if (!backgroundImage.isNull()) {
        p.drawPixmap(rect(), backgroundImage);
    } else {
        p.fillRect(rect(), Qt::black);
    }
}