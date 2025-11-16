#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>

namespace Ui { class LoginScreen; }

class LoginScreen : public QWidget {
    Q_OBJECT
public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

    void displayError(const QString &message);

signals:
    void connectAttempted(const QString& ip, const QString& port, const QString& name);

private slots:
    void on_btn_conectar_clicked();

private:
    Ui::LoginScreen *ui;
};

#endif // LOGINSCREEN_H
