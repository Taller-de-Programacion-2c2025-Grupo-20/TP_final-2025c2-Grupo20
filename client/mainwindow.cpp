#include "mainwindow.h"

#include <QDebug>
#include <stdexcept>

#include "ui_mainwindow.h"

MainWindow::MainWindow(const std::string& host, const std::string& port, QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        serverIp(QString::fromStdString(host)),
        serverPort(QString::fromStdString(port)) {
    ui->setupUi(this);
    loginScreen = new LoginScreen(this);
    ui->stackedWidget->addWidget(loginScreen);
    lobbyScreen = new LobbyScreen(this);
    ui->stackedWidget->addWidget(lobbyScreen);

    resultScreen = new ResultScreen(this);
    ui->stackedWidget->addWidget(resultScreen);

    ui->stackedWidget->setCurrentWidget(loginScreen);
    connect(loginScreen, &LoginScreen::connectAttempted, this, &MainWindow::onLoginAttempt);

    connect(lobbyScreen, &LobbyScreen::startGame, this, &MainWindow::on_gameStarted);

    connect(lobbyScreen, &LobbyScreen::serverDisconnected, this, [this]() { this->close(); });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onLoginAttempt(const QString& name) {
    loginScreen->setEnabled(false);
    loginScreen->displayError("Conectando...");

    try {
        client = std::make_unique<Client>(serverIp.toStdString().c_str(),
                                          serverPort.toStdString().c_str());

        connect(&(client->getReceiver()), &ClientReceiver::loginSuccess, this,
                &MainWindow::handleLoginSuccess);

        connect(&(client->getReceiver()), &ClientReceiver::loginFailed, this,
                &MainWindow::handleLoginFailed);

        connect(&(client->getReceiver()), &ClientReceiver::gameStarted, this,
                &MainWindow::on_gameStarted);

        client->start_threads();
        client->send_login_request(name.toStdString());

    } catch (const std::exception& e) {
        handleLoginFailed();
    }
}

void MainWindow::handleLoginSuccess() {
    loginScreen->displayError("¡Conectado!");
    lobbyScreen->setClient(client.get());
    ui->stackedWidget->setCurrentWidget(lobbyScreen);
}

void MainWindow::handleLoginFailed() {
    loginScreen->setEnabled(true);
    loginScreen->displayError("Error: no se pudo autenticar.");
    client.reset();
}

void MainWindow::on_gameStarted() {
    std::map<uint8_t, QString> names = lobbyScreen->getPlayerNamesMap();
    emit startGameSignal(client.get(), names);
}

void MainWindow::showResults(const std::vector<PlayerResultDTO>& results,
                             const std::map<uint8_t, QString>& names, uint8_t myId) {

    resultScreen->setResults(results, names, myId);

    ui->stackedWidget->setCurrentWidget(resultScreen);
}
