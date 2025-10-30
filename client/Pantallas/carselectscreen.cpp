#include "carselectscreen.h"
#include "ui_carselectscreen.h"

CarSelectScreen::CarSelectScreen(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CarSelectScreen)
{
    ui->setupUi(this);
}

CarSelectScreen::~CarSelectScreen()
{
    delete ui;
}
