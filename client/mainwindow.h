#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Pantallas/loginscreen.h"
#include "Pantallas/lobbyscreen.h"
#include "client.h"
//#include "./Pantallas/carselectscreen.h"

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

private slots:
    // este slot maneja el intento de conexion
    void handleLoginAttempt(const QString &ip, const QString &name);

private:
    Ui::MainWindow *ui;
    LoginScreen *pantallaLogin;
    LobbyScreen *pantallaLobby;

    Client *client = nullptr;
};

#endif // MAINWINDOW_H