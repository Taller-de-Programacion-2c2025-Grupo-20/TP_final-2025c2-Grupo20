#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QFutureWatcher>
#include <memory>
#include "Socket.h"

namespace Ui {
class LoginScreen;
}

// Devuelvo el resultado del hilo con QPair
// Par<PunteroAlSocket, MensajeDeError>
using LoginResult = QPair<std::unique_ptr<Socket>, QString>;

class LoginScreen : public QWidget
{
    Q_OBJECT

public:
    explicit LoginScreen(QWidget *parent = nullptr);
    ~LoginScreen();

    // Función para que MainWindow "tome" el socket una vez conectado
    std::unique_ptr<Socket> takeSocket();

signals:
    void loginExitoso(); // El MainWindow se conectará a esto

private slots:
    // Slot que se ejecuta cuando el botón es clickeado
    void on_btn_conectar_clicked();

    // Slot que se ejecuta en el hilo GUI cuando el hilo de red termina
    void onLoginFinished();

private:
    Ui::LoginScreen *ui;

    // El socket del cliente.
    // Utilizo unique_ptr porque Socket es movible (no copiable)
    std::unique_ptr<Socket> socket;

    // Objeto que "vigila" el hilo de conexión
    QFutureWatcher<LoginResult> loginWatcher;
};

#endif // LOGINSCREEN_H
