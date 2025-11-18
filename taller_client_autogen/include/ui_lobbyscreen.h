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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LobbyScreen
{
public:
    QLabel *title;
    QStackedWidget *stackedWidget_Lobby;
    QWidget *page_WaitingRoom;
    QListWidget *playerListWidget;
    QPushButton *startButton;
    QWidget *page_Selection;
    QPushButton *createButton;
    QLineEdit *matchNameEdit;
    QPushButton *joinButton;
    QListWidget *matchListWidget;

    void setupUi(QWidget *LobbyScreen)
    {
        if (LobbyScreen->objectName().isEmpty())
            LobbyScreen->setObjectName("LobbyScreen");
        LobbyScreen->resize(751, 536);
        title = new QLabel(LobbyScreen);
        title->setObjectName("title");
        title->setGeometry(QRect(350, 80, 121, 31));
        title->setStyleSheet(QString::fromUtf8("\n"
"font: 500 11pt \"Ubuntu Sans\";"));
        stackedWidget_Lobby = new QStackedWidget(LobbyScreen);
        stackedWidget_Lobby->setObjectName("stackedWidget_Lobby");
        stackedWidget_Lobby->setGeometry(QRect(120, 120, 541, 411));
        page_WaitingRoom = new QWidget();
        page_WaitingRoom->setObjectName("page_WaitingRoom");
        playerListWidget = new QListWidget(page_WaitingRoom);
        playerListWidget->setObjectName("playerListWidget");
        playerListWidget->setGeometry(QRect(160, 110, 256, 192));
        startButton = new QPushButton(page_WaitingRoom);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(230, 60, 111, 26));
        stackedWidget_Lobby->addWidget(page_WaitingRoom);
        page_Selection = new QWidget();
        page_Selection->setObjectName("page_Selection");
        createButton = new QPushButton(page_Selection);
        createButton->setObjectName("createButton");
        createButton->setGeometry(QRect(230, 30, 111, 26));
        matchNameEdit = new QLineEdit(page_Selection);
        matchNameEdit->setObjectName("matchNameEdit");
        matchNameEdit->setGeometry(QRect(220, 60, 131, 26));
        matchNameEdit->setStyleSheet(QString::fromUtf8("nombre...\n"
""));
        joinButton = new QPushButton(page_Selection);
        joinButton->setObjectName("joinButton");
        joinButton->setGeometry(QRect(230, 120, 111, 26));
        matchListWidget = new QListWidget(page_Selection);
        matchListWidget->setObjectName("matchListWidget");
        matchListWidget->setGeometry(QRect(160, 160, 256, 192));
        stackedWidget_Lobby->addWidget(page_Selection);

        retranslateUi(LobbyScreen);

        stackedWidget_Lobby->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LobbyScreen);
    } // setupUi

    void retranslateUi(QWidget *LobbyScreen)
    {
        LobbyScreen->setWindowTitle(QCoreApplication::translate("LobbyScreen", "Form", nullptr));
        title->setText(QCoreApplication::translate("LobbyScreen", "NEED FOR SPEED", nullptr));
        startButton->setText(QCoreApplication::translate("LobbyScreen", "Iniciar partida", nullptr));
        createButton->setText(QCoreApplication::translate("LobbyScreen", "Crear partida", nullptr));
#if QT_CONFIG(whatsthis)
        matchNameEdit->setWhatsThis(QCoreApplication::translate("LobbyScreen", "<html><head/><body><p>nombre...</p><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        matchNameEdit->setText(QString());
        joinButton->setText(QCoreApplication::translate("LobbyScreen", "Unirse a partida", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LobbyScreen: public Ui_LobbyScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBYSCREEN_H
