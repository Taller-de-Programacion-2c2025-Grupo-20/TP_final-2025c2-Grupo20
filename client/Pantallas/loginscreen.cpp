#include "loginscreen.h"
#include "ui_loginscreen.h"

#include <QStyleOption>
#include <QPainter>
#include <QStyle>

LoginScreen::LoginScreen(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::LoginScreen) {
    ui->setupUi(this);
    this->setObjectName("LoginScreen");

    this->setStyleSheet(
        "#LoginScreen { "
        "   border-image: url(:Pantallas/fondo_login.png)"
        "}"
    );
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

void LoginScreen::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}