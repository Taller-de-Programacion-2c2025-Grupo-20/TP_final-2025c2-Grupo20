#include <memory>
#include <QApplication>
#include <QDebug> 
#include "mainwindow.h"
#include "client.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Client* client_ptr_for_game = nullptr;
    MainWindow w(nullptr);

    client_ptr_for_game = client;
    app.quit(); 

    w.show();
    app.exec();

    if (client_ptr_for_game) {
        return client_ptr_for_game->runGame(); 
    }
    
    return EXIT_SUCCESS;
}