
#include <fstream>
#include <iostream>

#include "client.h"

// #include "mainwindow.h"
// #include <QApplication>

// int main(int argc, char *argv[]) {
//     QApplication a(argc, argv);
//     MainWindow w;
//     w.show();
//     return a.exec();
// }


// int main(int argc, char* argv[]) {

//     if (argc != 3) {
//         std::cerr << "Bad program call. Expected " << argv[0] << " <servname> <servicio> \n";
//         return EXIT_FAILURE;
//     }

//     try {

//         Client client(argv[1], argv[2]);
//         return client.run();

//     } catch (const std::exception& e) {
//         std::cerr << e.what() << '\n';
//         return EXIT_FAILURE;
//     } catch (...) {
//         std::cerr << "Unknown error\n";
//         return EXIT_FAILURE;
//     }
// }

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        std::cerr << "Cantidad de parametros incorrecta\n";
        return EXIT_FAILURE;
    }
    Client client(argv[1], argv[2]);
    client.runClient();
    return EXIT_SUCCESS;
}