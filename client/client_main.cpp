#include <memory>
#include <QApplication>
#include "mainwindow.h"
#include "client.h"

int main(int argc, char* argv[]) {  // importante: char* (no const*)
    QApplication app(argc, argv);

    std::unique_ptr<Client> client_for_game;

    MainWindow w(nullptr);
    QObject::connect(&w, &MainWindow::loginOk, [&](Client* readyClient){
        client_for_game.reset(readyClient);  // tomamos ownership
        app.quit();                          // cerramos Qt para lanzar SDL
    });
    QObject::connect(&w, &MainWindow::quitApp, [&](){
        app.quit();
    });

    w.show();
    app.exec();

    if (client_for_game) {
        return client_for_game->runClient(); // acá arranca SDL
    }
    return EXIT_SUCCESS;
}
