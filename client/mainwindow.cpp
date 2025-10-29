#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
    // Configura la interfaz de usuario (la infla desde mainwindow.ui)
    ui->setupUi(this);

            // 1. Crea tu pantalla de login
            //    (le pasamos 'this' para que sea "hija" de MainWindow
            //     y se destruya automáticamente cuando se cierre la ventana)
    pantallaLogin = new LoginScreen(this);

            // 2. Agrega la pantalla de login al Stacked Widget de tu UI
            //    (Asegúrate de que tu mainwindow.ui tenga un QStackedWidget
            //     con el nombre 'stackedWidget')
    ui->stackedWidget->addWidget(pantallaLogin);

            // 3. Muestra la pantalla de login como la pantalla inicial
    ui->stackedWidget->setCurrentWidget(pantallaLogin);

            // 4. Conecta la señal de "login exitoso" (que definimos antes)
            //    a una función simple (lambda) que por ahora solo imprime
            //    un mensaje en la consola.
    connect(pantallaLogin, &LoginScreen::loginExitoso, this, [=]() {
        qDebug() << "¡Login exitoso! (Señal recibida en MainWindow)";

        // --- Aquí es donde, en el futuro ---
        // 1. Crearías la 'CarSelectScreen'
        // 2. La agregarías al 'stackedWidget'
        // 3. Harías 'stackedWidget->setCurrentWidget(seleccionAutoScreen);'
    });
}

MainWindow::~MainWindow()
{
    // Libera la memoria de la UI
    delete ui;
}
