#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "Pantallas/loginscreen.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void loginRequested(const std::string& host, const std::string& port, const std::string& username);
    
    void startGame(); 

public slots:
    void handleLoginSuccess(uint8_t player_id);
    void handleLoginFailed();

private slots:
    void onLoginAttempt(const QString& ip, const QString& port, const QString& name);

private:
    Ui::MainWindow *ui;
    LoginScreen *pantallaLogin;
};

#endif