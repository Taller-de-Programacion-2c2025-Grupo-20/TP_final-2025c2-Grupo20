#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginscreen.h"
#include "carselectscreen.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pantallaLogin = new LoginScreen(this); // 'this' lo hace hijo de MainWindow
    seleccionAutoScreen = new PantallaSeleccionAuto(this);


    ui->stackedWidget->addWidget(pantallaLogin);
    ui->stackedWidget->addWidget(seleccionAutoScreen);

    ui->stackedWidget->setCurrentWidget(pantallaLogin);

    connect(pantallaLogin, &LoginScreen::loginExitoso, this, [=]() {
        std::unique_ptr<Socket> socket = pantallaLogin->takeSocket();

        seleccionAutoScreen->setSocket(std::move(socket));

        ui->stackedWidget->setCurrentWidget(seleccionAutoScreen);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
