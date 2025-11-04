#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>

MainWindow::MainWindow(Client* /*clientPtr*/, QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {

    ui->setupUi(this);

    pantallaLogin = new LoginScreen(this);
    ui->stackedWidget->addWidget(pantallaLogin);
    ui->stackedWidget->setCurrentWidget(pantallaLogin);

    connect(pantallaLogin, &LoginScreen::connectAttempted,
            this, &MainWindow::handleLoginAttempt);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleLoginAttempt(const QString& ip, const QString& port, const QString& name) {
    try {
        auto client = std::make_unique<Client>(ip.toStdString().c_str(),
                                               port.toStdString().c_str());

        const bool ok = client->try_login(name.toStdString());
        if (ok) {
            emit loginOk(client.release());
            setEnabled(false);
        } else {
            pantallaLogin->displayError("Error: no se pudo autenticar.");
        }

    } catch (const std::exception& e) {
        pantallaLogin->displayError(QString("Error de conexión: ") + e.what());
    }
}
