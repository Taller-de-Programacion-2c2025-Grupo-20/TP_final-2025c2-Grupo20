#include "loginscreen.h"
#include "ui_loginscreen.h" // El archivo generado por uic

LoginScreen::LoginScreen(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::LoginScreen)
{
    ui->setupUi(this);
    // El slot on_btn_conectar_clicked se conecta automáticamente por la convención de nombres.
}

LoginScreen::~LoginScreen()
{
    delete ui;
}

void LoginScreen::displayError(const QString &message) {
    ui->label_error->setText(message);
}

void LoginScreen::on_btn_conectar_clicked()
{
    QString ip = ui->edit_ip_servidor->text();
    QString name = ui->edit_nombre->text();

    if (ip.isEmpty() || name.isEmpty()) {
        this->displayError("Debe ingresar una IP y un nombre de usuario.");
        return;
    }

    this->displayError("");

    emit connectAttempted(ip, name);
}