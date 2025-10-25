#ifndef CARSELECTSCREEN_H
#define CARSELECTSCREEN_H

#include <QWidget>

namespace Ui {
class CarSelectScreen;
}

class CarSelectScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CarSelectScreen(QWidget *parent = nullptr);
    ~CarSelectScreen();

private:
    Ui::CarSelectScreen *ui;
};

#endif // CARSELECTSCREEN_H
