#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {

    ui->setupUi(this);
    pantallaLogin = new LoginScreen(this);
    ui->stackedWidget->addWidget(pantallaLogin);
    pantallaLobby = new LobbyScreen(this);
    ui->stackedWidget->addWidget(pantallaLobby);
    ui->stackedWidget->setCurrentWidget(pantallaLogin);
    connect(pantallaLogin, &LoginScreen::connectAttempted,
            this, &MainWindow::onLoginAttempt);
            
    connect(pantallaLobby, &LobbyScreen::startGame, this, &MainWindow::startGame);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLoginAttempt(const QString& ip, const QString& port, const QString& name) {
    pantallaLogin->setEnabled(false);
    pantallaLogin->displayError("Conectando...");
    
    try {
        client = std::make_unique<Client>(ip.toStdString().c_str(),
                                          port.toStdString().c_str());

        connect(&(client->getReceiver()), &ClientReceiver::loginSuccess, 
                                     this, &MainWindow::handleLoginSuccess);
        
        connect(&(client->getReceiver()), &ClientReceiver::loginFailed,
                                     this, &MainWindow::handleLoginFailed);

        client->start_threads();
        client->send_login_request(name.toStdString());
        
    } catch (const std::exception& e) {
        handleLoginFailed();
    }
}

void MainWindow::handleLoginSuccess() {
    pantallaLogin->displayError("¡Conectado!");
    pantallaLobby->setClient(client.get());
    ui->stackedWidget->setCurrentWidget(pantallaLobby);
}

void MainWindow::handleLoginFailed() {
    pantallaLogin->setEnabled(true);
    pantallaLogin->displayError("Error: no se pudo autenticar.");
    client.reset();
}