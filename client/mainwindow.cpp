#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {

    ui->setupUi(this);

    pantallaLogin = new LoginScreen(this);
    ui->stackedWidget->addWidget(pantallaLogin);
    ui->stackedWidget->setCurrentWidget(pantallaLogin);

    // Conecta el botón de LoginScreen a un slot de MainWindow
    connect(pantallaLogin, &LoginScreen::connectAttempted,
            this, &MainWindow::onLoginAttempt);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Este slot se activa cuando el LoginScreen emite "connectAttempted"
void MainWindow::onLoginAttempt(const QString& ip, const QString& port, const QString& name) {
    // 1. Deshabilita la UI para evitar doble clic
    pantallaLogin->setEnabled(false);
    pantallaLogin->displayError("Conectando..."); // Muestra mensaje
    
    // 2. Retransmite la señal al 'main' para que cree el cliente
    emit loginRequested(ip.toStdString(), port.toStdString(), name.toStdString());
}

// Este slot se activa cuando el HILO RECEIVER emite "loginSuccess"
void MainWindow::handleLoginSuccess(uint8_t /*player_id*/) {
    // ¡Login exitoso! Emitimos la señal para empezar el juego
    emit startGame();
}

// Este slot se activa cuando el HILO RECEIVER emite "loginFailed"
void MainWindow::handleLoginFailed() {
    // El login falló, reactivamos la UI
    pantallaLogin->setEnabled(true);
    pantallaLogin->displayError("Error: no se pudo autenticar.");
}