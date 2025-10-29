#include "loginscreen.h"
#include "ui_loginscreen.h" 
#include <QDebug> 

LoginScreen::LoginScreen(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LoginScreen) 
{

    ui->setupUi(this);

    connect(ui->btn_conectar, &QPushButton::clicked, this, &LoginScreen::onConectarClicked);
}

LoginScreen::~LoginScreen()
{
    delete ui;
}

void LoginScreen::onConectarClicked()
{
    qDebug() << "Botón 'Conectar' presionado.";

    QString nombre = ui->edit_nombre->text();
    QString ip = ui->edit_ip_servidor->text();

    qDebug() << "Nombre ingresado:" << nombre;
    qDebug() << "IP ingresada:" << ip;

    ui->label_error->setText("Simulando conexión...");

}
