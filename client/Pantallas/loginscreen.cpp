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
    const QString name  = ui->edit_nombre->text().trimmed();

    if (name.isEmpty()) {
        displayError("Completa tu nombre de usuario.");
        return;
    }
    displayError("");
    emit connectAttempted(name);
}
