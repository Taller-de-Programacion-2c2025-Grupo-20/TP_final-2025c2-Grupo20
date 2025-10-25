//#include "common/foo.h"
#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    //return foo(4);
    QApplication app(argc, argv);

    MainWindow window;
    window.show();
    return app.exec();
}
