#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <memory>
#include "Pantallas/loginscreen.h"
#include "Pantallas/lobbyscreen.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const std::string& host, const std::string& port, QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void loginRequested(const std::string& username);
    void startGame(Client* client); 

public slots:
    void handleLoginSuccess();
    void handleLoginFailed();

private slots:
    void onLoginAttempt(const QString& name);
    void on_gameStarted();

private:
    Ui::MainWindow *ui;
    QString serverIp;
    QString serverPort;
    LoginScreen *pantallaLogin;
    LobbyScreen *pantallaLobby;
    std::unique_ptr<Client> client; 
};

#endif