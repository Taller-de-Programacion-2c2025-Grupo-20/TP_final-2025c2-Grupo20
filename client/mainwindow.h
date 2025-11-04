#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "Pantallas/loginscreen.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(Client* client = nullptr, QWidget* parent = nullptr);
    ~MainWindow();

signals:
    // Entregamos el Client* listo (ownership lo toma quien recibe)
    void loginOk(Client* client);
    void quitApp();

private slots:
    void handleLoginAttempt(const QString& ip, const QString& port, const QString& name);

private:
    Ui::MainWindow *ui;
    LoginScreen *pantallaLogin;

    std::unique_ptr<Client> client; // ownership mientras está en esta ventana
};

#endif // MAINWINDOW_H
