#include <memory>
#include <QApplication>
#include "mainwindow.h"
#include "client.h"
#include "game_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Client* client_ptr_for_game = nullptr;
    MainWindow w(nullptr);

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