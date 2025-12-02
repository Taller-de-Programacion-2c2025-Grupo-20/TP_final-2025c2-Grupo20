#include <memory>
#include <QApplication>
#include <QFontDatabase>
#include "mainwindow.h"
#include "client.h"
#include "../common/constants.h"
#include <map>
#include "game_window.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        qCritical("Uso: %s <host> <puerto>", argv[0]);
        return EXIT_FAILURE;
    }
    std::string host = argv[1];
    std::string port = argv[2];
    QApplication app(argc, argv);

    int id = QFontDatabase::addApplicationFont(FONT_FILE);
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    
    Client* client_ptr_for_game = nullptr;
    std::map<uint8_t, QString> player_names;

    MainWindow w(host, port, nullptr);
    QObject::connect(&w, &MainWindow::startGameSignal, 
        [&](Client* client_from_window, std::map<uint8_t, QString> names) {
            
            client_ptr_for_game = client_from_window;
            player_names = names;
            
            w.hide();
            app.quit();
        }
    );

    w.show();
    app.exec(); 
    if (client_ptr_for_game) {
        
        GameWindow gw(*client_ptr_for_game); 
        gw.runGame();
        if (client_ptr_for_game->hasFinishedGame()) {
            std::cout << "Termino la partida\n";
            w.showResults(client_ptr_for_game->getFinalState(), 
                          player_names, 
                          client_ptr_for_game->getMyPlayerId());

            w.show();
            return app.exec(); 
        }
    }
    
    return EXIT_SUCCESS;
}