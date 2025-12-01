#ifndef RESULTSCREEN_H
#define RESULTSCREEN_H

#include <QWidget>
#include <vector>
#include <map>
#include "../common/gameState.h"

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

private slots:
    void on_btnExit_clicked();

private:
    Ui::ResultScreen *ui;
};

#endif // RESULTSCREEN_H