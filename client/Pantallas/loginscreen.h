#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QFutureWatcher>
//#include <memory>

namespace Ui {
class LoginScreen;
}

class LoginScreen : public QWidget
{
    Q_OBJECT 

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();
    void displayError(const QString &message);

signals:
    // notifico a mainwindow q el usuario intento conectarse
    void connectAttempted(const QString &ip, const QString &name);

private slots:
    // conectado a btn_conectar
    void on_btn_conectar_clicked();

private:
    Ui::LoginScreen *ui;

};

#endif