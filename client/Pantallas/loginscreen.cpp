#include "loginscreen.h"
#include "ui_loginscreen.h"

LoginScreen::LoginScreen(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::LoginScreen) {
    ui->setupUi(this);
}

LoginScreen::~LoginScreen() {
    delete ui;
}

void LoginScreen::displayError(const QString &message) {
    ui->label_error->setText(message);
}

void LoginScreen::on_btn_conectar_clicked() {
    const QString ip    = ui->edit_ip_servidor->text().trimmed();
    const QString puerto= ui->edit_puerto->text().trimmed();
    const QString name  = ui->edit_nombre->text().trimmed();

    if (ip.isEmpty() || puerto.isEmpty() || name.isEmpty()) {
        displayError("Completá nombre, IP y puerto.");
        return;
    }
    displayError("");
    emit connectAttempted(ip, puerto, name);
}
