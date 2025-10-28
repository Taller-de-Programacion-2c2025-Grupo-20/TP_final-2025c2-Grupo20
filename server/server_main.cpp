#include <iostream>

#include "../common/constants.h"
#include "../common/socket.h"

#include "server.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Bad program call. Expected " << argv[0] << " <puerto>\n";
        return EXIT_FAILURE;
    }

    try {
        Server server(argv[1]);
        return server.run();

    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
}
