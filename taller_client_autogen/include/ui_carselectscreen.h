/********************************************************************************
** Form generated from reading UI file 'carselectscreen.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CARSELECTSCREEN_H
#define UI_CARSELECTSCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CarSelectScreen
{
public:

    void setupUi(QWidget *CarSelectScreen)
    {
        if (CarSelectScreen->objectName().isEmpty())
            CarSelectScreen->setObjectName("CarSelectScreen");
        CarSelectScreen->resize(400, 300);

        retranslateUi(CarSelectScreen);

        QMetaObject::connectSlotsByName(CarSelectScreen);
    } // setupUi

    void retranslateUi(QWidget *CarSelectScreen)
    {
        CarSelectScreen->setWindowTitle(QCoreApplication::translate("CarSelectScreen", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CarSelectScreen: public Ui_CarSelectScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CARSELECTSCREEN_H
