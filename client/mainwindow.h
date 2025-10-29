#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Pantallas/loginscreen.h"
//#include "./Pantallas/carselectscreen.h"
//#include ",,/common/socket.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    LoginScreen *pantallaLogin;
    //CarSelectScreen* seleccionAutoScreen;
};

#endif // MAINWINDOW_H
