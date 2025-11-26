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
#include <QtWidgets/QComboBox>
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
    QComboBox *mapComboBox;
    QLabel *title_2;
    QLabel *title_3;
    QComboBox *carComboBox;
    QLabel *label_mapPreview;
    QLabel *label_carPreview;
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
        title->setGeometry(QRect(330, 10, 121, 31));
        title->setStyleSheet(QString::fromUtf8("\n"
"font: 500 11pt \"Ubuntu Sans\";"));
        stackedWidget_Lobby = new QStackedWidget(LobbyScreen);
        stackedWidget_Lobby->setObjectName("stackedWidget_Lobby");
        stackedWidget_Lobby->setGeometry(QRect(30, 40, 701, 481));
        page_WaitingRoom = new QWidget();
        page_WaitingRoom->setObjectName("page_WaitingRoom");
        playerListWidget = new QListWidget(page_WaitingRoom);
        playerListWidget->setObjectName("playerListWidget");
        playerListWidget->setGeometry(QRect(260, 70, 201, 141));
        startButton = new QPushButton(page_WaitingRoom);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(300, 20, 111, 26));
        mapComboBox = new QComboBox(page_WaitingRoom);
        mapComboBox->setObjectName("mapComboBox");
        mapComboBox->setGeometry(QRect(40, 230, 161, 26));
        title_2 = new QLabel(page_WaitingRoom);
        title_2->setObjectName("title_2");
        title_2->setGeometry(QRect(60, 190, 121, 31));
        title_2->setStyleSheet(QString::fromUtf8("\n"
"font: 500 11pt \"Ubuntu Sans\";"));
        title_3 = new QLabel(page_WaitingRoom);
        title_3->setObjectName("title_3");
        title_3->setGeometry(QRect(540, 190, 121, 31));
        title_3->setStyleSheet(QString::fromUtf8("\n"
"font: 500 11pt \"Ubuntu Sans\";"));
        carComboBox = new QComboBox(page_WaitingRoom);
        carComboBox->setObjectName("carComboBox");
        carComboBox->setGeometry(QRect(510, 230, 161, 26));
        label_mapPreview = new QLabel(page_WaitingRoom);
        label_mapPreview->setObjectName("label_mapPreview");
        label_mapPreview->setGeometry(QRect(10, 270, 251, 221));
        label_mapPreview->setMinimumSize(QSize(200, 150));
        label_carPreview = new QLabel(page_WaitingRoom);
        label_carPreview->setObjectName("label_carPreview");
        label_carPreview->setGeometry(QRect(529, 280, 121, 101));
        label_carPreview->setMinimumSize(QSize(100, 75));
        stackedWidget_Lobby->addWidget(page_WaitingRoom);
        page_Selection = new QWidget();
        page_Selection->setObjectName("page_Selection");
        createButton = new QPushButton(page_Selection);
        createButton->setObjectName("createButton");
        createButton->setGeometry(QRect(300, 50, 111, 26));
        matchNameEdit = new QLineEdit(page_Selection);
        matchNameEdit->setObjectName("matchNameEdit");
        matchNameEdit->setGeometry(QRect(290, 80, 131, 26));
        matchNameEdit->setStyleSheet(QString::fromUtf8("nombre...\n"
""));
        joinButton = new QPushButton(page_Selection);
        joinButton->setObjectName("joinButton");
        joinButton->setGeometry(QRect(300, 150, 111, 26));
        matchListWidget = new QListWidget(page_Selection);
        matchListWidget->setObjectName("matchListWidget");
        matchListWidget->setGeometry(QRect(230, 190, 256, 192));
        stackedWidget_Lobby->addWidget(page_Selection);

        retranslateUi(LobbyScreen);

        stackedWidget_Lobby->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LobbyScreen);
    } // setupUi

    void retranslateUi(QWidget *LobbyScreen)
    {
        LobbyScreen->setWindowTitle(QCoreApplication::translate("LobbyScreen", "Form", nullptr));
        title->setText(QCoreApplication::translate("LobbyScreen", "NEED FOR SPEED", nullptr));
        startButton->setText(QCoreApplication::translate("LobbyScreen", "Iniciar partida", nullptr));
        title_2->setText(QCoreApplication::translate("LobbyScreen", "Seleccionar mapa", nullptr));
        title_3->setText(QCoreApplication::translate("LobbyScreen", "Seleccionar auto", nullptr));
        label_mapPreview->setText(QString());
        label_carPreview->setText(QString());
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
