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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginScreen
{
public:
    QGridLayout *gridLayout_3;
    QLabel *label_titulo;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer_5;
    QLabel *label_ip_servidor;
    QSpacerItem *verticalSpacer_3;
    QLineEdit *edit_nombre;
    QLabel *label_nombre;
    QLabel *label_error;
    QSpacerItem *verticalSpacer;
    QPushButton *btn_conectar;
    QLineEdit *edit_ip_servidor;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_6;

    void setupUi(QWidget *LoginScreen)
    {
        if (LoginScreen->objectName().isEmpty())
            LoginScreen->setObjectName("LoginScreen");
        LoginScreen->resize(800, 600);
        LoginScreen->setMinimumSize(QSize(800, 600));
        gridLayout_3 = new QGridLayout(LoginScreen);
        gridLayout_3->setObjectName("gridLayout_3");
        label_titulo = new QLabel(LoginScreen);
        label_titulo->setObjectName("label_titulo");
        QFont font;
        font.setPointSize(22);
        font.setBold(true);
        label_titulo->setFont(font);
        label_titulo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_3->addWidget(label_titulo, 7, 0, 1, 2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_4, 1, 0, 1, 2);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_5, 0, 0, 1, 2);

        label_ip_servidor = new QLabel(LoginScreen);
        label_ip_servidor->setObjectName("label_ip_servidor");
        label_ip_servidor->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_3->addWidget(label_ip_servidor, 10, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 80, QSizePolicy::Policy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_3, 8, 0, 1, 2);

        edit_nombre = new QLineEdit(LoginScreen);
        edit_nombre->setObjectName("edit_nombre");

        gridLayout_3->addWidget(edit_nombre, 9, 1, 1, 1);

        label_nombre = new QLabel(LoginScreen);
        label_nombre->setObjectName("label_nombre");
        label_nombre->setAlignment(Qt::AlignmentFlag::AlignRight|Qt::AlignmentFlag::AlignTrailing|Qt::AlignmentFlag::AlignVCenter);

        gridLayout_3->addWidget(label_nombre, 9, 0, 1, 1);

        label_error = new QLabel(LoginScreen);
        label_error->setObjectName("label_error");
        label_error->setStyleSheet(QString::fromUtf8("color: red;"));
        label_error->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_3->addWidget(label_error, 13, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer, 2, 0, 1, 2);

        btn_conectar = new QPushButton(LoginScreen);
        btn_conectar->setObjectName("btn_conectar");

        gridLayout_3->addWidget(btn_conectar, 11, 1, 1, 1);

        edit_ip_servidor = new QLineEdit(LoginScreen);
        edit_ip_servidor->setObjectName("edit_ip_servidor");

        gridLayout_3->addWidget(edit_ip_servidor, 10, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_2, 14, 0, 1, 2);

        verticalSpacer_6 = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(verticalSpacer_6, 15, 0, 1, 2);


        retranslateUi(LoginScreen);

        QMetaObject::connectSlotsByName(LoginScreen);
    } // setupUi

    void retranslateUi(QWidget *LoginScreen)
    {
        LoginScreen->setWindowTitle(QCoreApplication::translate("LoginScreen", "Form", nullptr));
        label_titulo->setText(QCoreApplication::translate("LoginScreen", "Need for Speed", nullptr));
        label_ip_servidor->setText(QCoreApplication::translate("LoginScreen", "IP Servidor:", nullptr));
        label_nombre->setText(QCoreApplication::translate("LoginScreen", "Nombre:", nullptr));
        label_error->setText(QString());
        btn_conectar->setText(QCoreApplication::translate("LoginScreen", "Conectar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginScreen: public Ui_LoginScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINSCREEN_H
