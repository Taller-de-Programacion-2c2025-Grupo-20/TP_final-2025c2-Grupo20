#include "loginscreen.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include "ui_loginscreen.h"

LoginScreen::LoginScreen(QWidget* parent): QWidget(parent), ui(new Ui::LoginScreen) {
    ui->setupUi(this);
    this->setObjectName("LoginScreen");

    this->setStyleSheet("#LoginScreen { "
                        "   border-image: url(:/Pantallas/fondo_login.png) 0 0 0 0 stretch stretch;"
                        "   font-family: 'Press Start 2P';"
                        "}");
}

LoginScreen::~LoginScreen() { delete ui; }

void LoginScreen::displayError(const QString& message) { ui->label_error->setText(message); }

void LoginScreen::on_btn_conectar_clicked() {
    const QString name = ui->edit_nombre->text().trimmed();

    if (name.isEmpty()) {
        displayError("Completa tu nombre de usuario.");
        return;
    }
    displayError("");
    emit connectAttempted(name);
}

void LoginScreen::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
