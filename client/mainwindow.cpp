#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <QDebug>
#include <stdexcept>


MainWindow::MainWindow(Client* clientPtr, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    client(clientPtr) {
    
    ui->setupUi(this);

    pantallaLogin = new LoginScreen(this);
    ui->stackedWidget->addWidget(pantallaLogin);

    pantallaLobby = new LobbyScreen(this);
    ui->stackedWidget->addWidget(pantallaLobby);

    ui->stackedWidget->setCurrentWidget(pantallaLogin);

    connect(pantallaLogin, &LoginScreen::connectAttempted, this, &MainWindow::handleLoginAttempt);
}

MainWindow::~MainWindow() {
    delete client;
    delete ui;
}

void MainWindow::handleLoginAttempt(const QString /*&ip*/, const QString &name) {
    try {
        

        //bool success = client->getProtocol().sendName(name.toStdString());
        bool success = client->try_login(name.toStdString());

        if (success) {
            pantallaLogin->displayError("");

            ui->stackedWidget->setCurrentWidget(pantallaLobby);

        } else {
            // Fallo de protocolo: Nombre rechazado
            pantallaLogin->displayError("Error: Nombre de usuario inválido o en uso");
            delete client;
            client = nullptr;
        }

    } catch (const std::exception& e) {
        // Fallo de conexión (captura la excepción lanzada por ClientProtocolMock
        QString errorMsg = QString("Error de conexión en el login") + e.what();
        pantallaLogin->displayError(errorMsg);
        delete client;
        client = nullptr;
    }
}