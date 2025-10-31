/********************************************************************************
** Form generated from reading UI file 'lobbyscreen.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOBBYSCREEN_H
#define UI_LOBBYSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LobbyScreen
{
public:

    void setupUi(QWidget *LobbyScreen)
    {
        if (LobbyScreen->objectName().isEmpty())
            LobbyScreen->setObjectName("LobbyScreen");
        LobbyScreen->resize(400, 300);

        retranslateUi(LobbyScreen);

        QMetaObject::connectSlotsByName(LobbyScreen);
    } // setupUi

    void retranslateUi(QWidget *LobbyScreen)
    {
        LobbyScreen->setWindowTitle(QCoreApplication::translate("LobbyScreen", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LobbyScreen: public Ui_LobbyScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBYSCREEN_H
