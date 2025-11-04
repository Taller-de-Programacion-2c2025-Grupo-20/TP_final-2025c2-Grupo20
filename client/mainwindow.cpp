#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>
#include <utility>

MainWindow::MainWindow(Client* clientPtr, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      client(clientPtr ? std::unique_ptr<Client>(clientPtr) : nullptr) {

    ui->setupUi(this);

    // Solo pantalla de login
    pantallaLogin = new LoginScreen(this);
    ui->stackedWidget->addWidget(pantallaLogin);
    ui->stackedWidget->setCurrentWidget(pantallaLogin);

    // ip, puerto, nombre
    connect(pantallaLogin, &LoginScreen::connectAttempted,
            this, &MainWindow::handleLoginAttempt);
}

MainWindow::~MainWindow() {
    delete ui; // unique_ptr de client se libera solo
}

void MainWindow::handleLoginAttempt(const QString& ip, const QString& port, const QString& name) {
    try {
        const std::string host = ip.toStdString();
        const std::string prt  = port.toStdString();

        // 1) Crear Client con host/puerto de la UI
        client = std::make_unique<Client>(host.c_str(), prt.c_str());

        // 2) Intentar login con el nombre
        const bool success = client->try_login(name.toStdString());

        if (success) {
            pantallaLogin->displayError("");

            // 3) Transferir ownership y cerrar Qt (el main lanza SDL)
            Client* raw = client.release();
            emit loginOk(raw);
            setEnabled(false); // evitar más interacción mientras se cierra
        } else {
            pantallaLogin->displayError("Error: no se pudo autenticar (revisá IP/puerto o el protocolo).");
            client.reset();
        }

    } catch (const std::exception& e) {
        pantallaLogin->displayError(QString("Error de conexión: ") + e.what());
        client.reset();
    }
}
