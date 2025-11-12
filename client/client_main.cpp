#include <memory>
#include <QApplication>
#include "mainwindow.h"
#include "client.h" // Asumiendo que Client posee a Sender y Receiver

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    std::unique_ptr<Client> client_for_game;
    MainWindow w(nullptr);

    // 1. Conectamos la UI al main: La UI solicita un login
    QObject::connect(&w, &MainWindow::loginRequested, 
        [&](const std::string& host, const std::string& port, const std::string& username) {
        
        try {
            // 2. Creamos el cliente (esto conecta el socket)
            client_for_game = std::make_unique<Client>(host.c_str(), port.c_str());
            
            // 3. Conectamos el Hilo Receiver a la UI
            // (El Receiver le avisará a la UI cuando el login termine)
            QObject::connect(&(client_for_game->getReceiver()), &ClientReceiver::loginSuccess, 
                             &w, &MainWindow::handleLoginSuccess);
            
            QObject::connect(&(client_for_game->getReceiver()), &ClientReceiver::loginFailed,
                             &w, &MainWindow::handleLoginFailed);

            // 4. Iniciamos los hilos de red
            client_for_game->start_threads();

            // 5. Enviamos la solicitud de login (ASINCRÓNICO, no bloquea)
            client_for_game->send_login_request(username);
            
        } catch (const std::exception& e) {
            // Error al crear el Client (ej: no se pudo conectar)
            w.handleLoginFailed();
        }
    });

    // 6. Conectamos la UI al main: El login fue exitoso, listo para jugar
    QObject::connect(&w, &MainWindow::startGame, [&]() {
        app.quit(); // Cerramos la UI de Qt
    });

    w.show();
    app.exec(); // Bucle de Qt

    // 7. Si el login fue exitoso, client_for_game no será null
    if (client_for_game) {
        return client_for_game->runGame(); // Bucle de SDL
    }
    
    return EXIT_SUCCESS;
}