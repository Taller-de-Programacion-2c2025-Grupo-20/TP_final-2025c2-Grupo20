/********************************************************************************
** Form generated from reading UI file 'loginscreen.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINSCREEN_H
#define UI_LOGINSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginScreen
{
public:
    QGridLayout *gridLayout;
    QLabel *label_titulo;
    QLabel *label_nombre;
    QLineEdit *edit_nombre;
    QLabel *label_ip_servidor;
    QLineEdit *edit_ip_servidor;
    QLabel *label_puerto;
    QLineEdit *edit_puerto;
    QPushButton *btn_conectar;
    QLabel *label_error;

    void setupUi(QWidget *LoginScreen)
    {
        if (LoginScreen->objectName().isEmpty())
            LoginScreen->setObjectName("LoginScreen");
        LoginScreen->resize(800, 600);
        LoginScreen->setMinimumSize(QSize(800, 600));
        gridLayout = new QGridLayout(LoginScreen);
        gridLayout->setObjectName("gridLayout");
        label_titulo = new QLabel(LoginScreen);
        label_titulo->setObjectName("label_titulo");
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        label_titulo->setFont(font);
        label_titulo->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_titulo, 0, 0, 1, 2);

        label_nombre = new QLabel(LoginScreen);
        label_nombre->setObjectName("label_nombre");
        label_nombre->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout->addWidget(label_nombre, 1, 0, 1, 1);

        edit_nombre = new QLineEdit(LoginScreen);
        edit_nombre->setObjectName("edit_nombre");

        gridLayout->addWidget(edit_nombre, 1, 1, 1, 1);

        label_ip_servidor = new QLabel(LoginScreen);
        label_ip_servidor->setObjectName("label_ip_servidor");
        label_ip_servidor->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout->addWidget(label_ip_servidor, 2, 0, 1, 1);

        edit_ip_servidor = new QLineEdit(LoginScreen);
        edit_ip_servidor->setObjectName("edit_ip_servidor");

        gridLayout->addWidget(edit_ip_servidor, 2, 1, 1, 1);

        label_puerto = new QLabel(LoginScreen);
        label_puerto->setObjectName("label_puerto");
        label_puerto->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        gridLayout->addWidget(label_puerto, 3, 0, 1, 1);

        edit_puerto = new QLineEdit(LoginScreen);
        edit_puerto->setObjectName("edit_puerto");

        gridLayout->addWidget(edit_puerto, 3, 1, 1, 1);

        btn_conectar = new QPushButton(LoginScreen);
        btn_conectar->setObjectName("btn_conectar");

        gridLayout->addWidget(btn_conectar, 4, 0, 1, 2);

        label_error = new QLabel(LoginScreen);
        label_error->setObjectName("label_error");
        label_error->setStyleSheet(QString::fromUtf8("color: red;"));
        label_error->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label_error, 5, 0, 1, 2);


        retranslateUi(LoginScreen);

        QMetaObject::connectSlotsByName(LoginScreen);
    } // setupUi

    void retranslateUi(QWidget *LoginScreen)
    {
        LoginScreen->setWindowTitle(QCoreApplication::translate("LoginScreen", "Login", nullptr));
        label_titulo->setText(QCoreApplication::translate("LoginScreen", "Need for Speed", nullptr));
        label_nombre->setText(QCoreApplication::translate("LoginScreen", "Nombre:", nullptr));
        label_ip_servidor->setText(QCoreApplication::translate("LoginScreen", "IP Servidor:", nullptr));
        edit_ip_servidor->setPlaceholderText(QCoreApplication::translate("LoginScreen", "localhost o 127.0.0.1", nullptr));
        label_puerto->setText(QCoreApplication::translate("LoginScreen", "Puerto:", nullptr));
        edit_puerto->setPlaceholderText(QCoreApplication::translate("LoginScreen", "p.ej. 5555", nullptr));
        btn_conectar->setText(QCoreApplication::translate("LoginScreen", "Conectar", nullptr));
        label_error->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginScreen: public Ui_LoginScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSCREEN_H
