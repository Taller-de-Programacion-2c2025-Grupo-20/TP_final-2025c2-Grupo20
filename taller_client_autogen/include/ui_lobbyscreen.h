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
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LobbyScreen
{
public:
    QPushButton *readyButton;
    QPushButton *startButton;
    QListWidget *playerListWidget;
    QLabel *players;
    QLabel *title;

    void setupUi(QWidget *LobbyScreen)
    {
        if (LobbyScreen->objectName().isEmpty())
            LobbyScreen->setObjectName("LobbyScreen");
        LobbyScreen->resize(751, 536);
        readyButton = new QPushButton(LobbyScreen);
        readyButton->setObjectName("readyButton");
        readyButton->setGeometry(QRect(310, 200, 121, 26));
        startButton = new QPushButton(LobbyScreen);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(310, 240, 121, 26));
        playerListWidget = new QListWidget(LobbyScreen);
        playerListWidget->setObjectName("playerListWidget");
        playerListWidget->setGeometry(QRect(240, 330, 256, 192));
        players = new QLabel(LobbyScreen);
        players->setObjectName("players");
        players->setGeometry(QRect(300, 300, 151, 18));
        title = new QLabel(LobbyScreen);
        title->setObjectName("title");
        title->setGeometry(QRect(310, 90, 121, 31));
        title->setStyleSheet(QString::fromUtf8("\n"
"font: 500 11pt \"Ubuntu Sans\";"));

        retranslateUi(LobbyScreen);

        QMetaObject::connectSlotsByName(LobbyScreen);
    } // setupUi

    void retranslateUi(QWidget *LobbyScreen)
    {
        LobbyScreen->setWindowTitle(QCoreApplication::translate("LobbyScreen", "Form", nullptr));
        readyButton->setText(QCoreApplication::translate("LobbyScreen", "Listo!", nullptr));
        startButton->setText(QCoreApplication::translate("LobbyScreen", "Iniciar partida", nullptr));
        players->setText(QCoreApplication::translate("LobbyScreen", "Jugadores conectados", nullptr));
        title->setText(QCoreApplication::translate("LobbyScreen", "NEED FOR SPEED", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LobbyScreen: public Ui_LobbyScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBYSCREEN_H
