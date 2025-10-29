// #ifndef LOGINSCREEN_H
// #define LOGINSCREEN_H

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

signals:
    void loginExitoso();

private slots:
    void onConectarClicked();

private:
    Ui::LoginScreen *ui;
};

// endif
