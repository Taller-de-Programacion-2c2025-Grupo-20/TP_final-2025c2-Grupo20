#include <memory>
#include <QApplication>
#include "mainwindow.h"
#include "client.h"
#include "game_window.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        qCritical("Uso: %s <host> <puerto>", argv[0]);
        return EXIT_FAILURE;
    }
    std::string host = argv[1];
    std::string port = argv[2];
    QApplication app(argc, argv);
    Client* client_ptr_for_game = nullptr;
    MainWindow w(host, port, nullptr);

    QObject::connect(&w, &MainWindow::startGame, [&](Client* client_from_window) {
        client_ptr_for_game = client_from_window;
        app.quit(); 
    });

    w.show();
    app.exec();

    if (client_ptr_for_game) {
        GameWindow gw(*client_ptr_for_game); 
        return gw.runGame();                
    }
    
    return EXIT_SUCCESS;
}