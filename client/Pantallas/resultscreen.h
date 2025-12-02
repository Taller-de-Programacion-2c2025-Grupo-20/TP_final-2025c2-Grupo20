#ifndef RESULTSCREEN_H
#define RESULTSCREEN_H

#include <QWidget>
#include <QPixmap>
#include <QPainter> 
#include <vector>
#include <map>
#include "../common/dto.h"

namespace Ui {
class ResultScreen;
}

class ResultScreen : public QWidget {
    Q_OBJECT

public:
    explicit ResultScreen(QWidget *parent = nullptr);
    ~ResultScreen();

    void setResults(const std::vector<PlayerResultDTO>& results, 
                    const std::map<uint8_t, QString>& idToNameMap,
                    uint8_t myPlayerId);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_btnExit_clicked();

signals:
    void returnToLobby(); 
    void closeApplication();

private:
    Ui::ResultScreen *ui;
    QPixmap backgroundImage; 
};

#endif // RESULTSCREEN_H
