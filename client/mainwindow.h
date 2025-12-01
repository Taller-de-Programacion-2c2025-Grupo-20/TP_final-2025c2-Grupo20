#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <memory>
#include "Pantallas/loginscreen.h"
#include "Pantallas/lobbyscreen.h"
#include "Pantallas/resultscreen.h"
#include "game_window.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const std::string& host, const std::string& port, QWidget* parent = nullptr);
    ~MainWindow();
    void showResults(const std::vector<PlayerResultDTO>& results, 
                     const std::map<uint8_t, QString>& names, 
                     uint8_t myId);

signals:
    void loginRequested(const std::string& username);
    void startGameSignal(Client* client, std::map<uint8_t, QString> names);

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
    LoginScreen *loginScreen;
    LobbyScreen *lobbyScreen;
    ResultScreen *resultScreen;
    std::unique_ptr<Client> client; 
};

#endif