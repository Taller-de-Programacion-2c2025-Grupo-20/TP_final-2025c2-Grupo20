#include "loginscreen.h"
#include "ui_loginscreen.h"

#include <QtConcurrent/QtConcurrent> // Para QtConcurrent::run
#include <QStringList>
#include <QByteArray>
#include <QDataStream> // para armar paquetes binarios

LoginScreen::LoginScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginScreen)
{
    ui->setupUi(this);

    // Conectamos la señal 'finished' del watcher a nuestro slot
    connect(&loginWatcher, &QFutureWatcher<LoginResult>::finished,
            this, &LoginScreen::onLoginFinished);
}

LoginScreen::~LoginScreen()
{
    delete ui;
}

// El botón Conectar solo LANZA el hilo de trabajo
void LoginScreen::on_btn_conectar_clicked()
{
    // 1. Obtener datos y validar UI
    QString nombre = ui->edit_nombre->text().trimmed();
    QString ipCompleta = ui->edit_ip_servidor->text().trimmed();

    if (nombre.isEmpty() || ipCompleta.isEmpty()) {
        ui->label_error->setText("Error: Nombre e IP no pueden estar vacíos.");
        return;
    }

    QStringList parts = ipCompleta.split(':');
    if (parts.length() != 2) {
        ui->label_error->setText("Error: Formato de IP inválido. Use IP:PUERTO");
        return;
    }
    QString ip = parts[0];
    QString puerto = parts[1];

    // Deshabilitar UI mientras se conecta
    ui->label_error->setText("Conectando...");
    ui->btn_conectar->setEnabled(false);

    // Lanzo el trabajo en un hilo separado
    // Utilizo una función lambda para capturar las variables que necesitamos
    QFuture<LoginResult> future = QtConcurrent::run([=]() -> LoginResult {
        // Este codigo se ejecuta en un hilo separado
        // try {
        //     std::unique_ptr<Socket> tempSocket = std::make_unique<Socket>(
        //         ip.toStdString().c_str(),
        //         puerto.toStdString().c_str()
        //         );

        //     QByteArray packet;
        //     QDataStream out(&packet, QIODevice::WriteOnly);

        //     quint8 msgType = 0x01; // Login, lo podria hacer un enum
        //     QByteArray nombreUtf8 = nombre.toUtf8(); // Convertir a bytes
        //     quint8 nombreLen = (quint8)nombreUtf8.size();

        //     out << msgType;
        //     out << nombreLen;

        //     packet.append(nombreUtf8);

        //     // Enviar datos (Bloqueante)
        //     tempSocket->sendall(packet.constData(), packet.size());

        //     // Recibir respuesta (Bloqueante)
        //     quint8 responseCode;
        //     tempSocket->recvall(&responseCode, 1);

        //     if (responseCode == 0x01) {
        //         // Movemos el socket al resultado y no ponemos error
        //         return qMakePair(std::move(tempSocket), QString(""));
        //     } else if (responseCode == 0x02) {
        //         return qMakePair(nullptr, QString("Error: Nombre de jugador ya en uso."));
        //     } else if (responseCode == 0x03) {
        //         return qMakePair(nullptr, QString("Error: La partida está llena."));
        //     } else {
        //         return qMakePair(nullptr, QString("Error: Respuesta desconocida del servidor."));
        //     }

        // } catch (const std::exception &e) {
        //     return qMakePair(nullptr, QString("Error de conexión: %1").arg(e.what()));
        //}); }
        try {
            // 1. Conexión (Bloqueante)
            std::unique_ptr<Socket> tempSocket = std::make_unique<Socket>(
                ip.toStdString().c_str(), 
                puerto.toStdString().c_str()
            );

            // 2. Crear instancia del protocolo
            ClientProtocol protocol;
            
            // 3. Usar el protocolo para enviar
            // (Usamos .get() para pasar el puntero crudo, 
            //  pero unique_ptr sigue siendo el dueño)
            protocol.send_login(*tempSocket.get(), nombre.toStdString());

            // 4. Usar el protocolo para recibir respuesta
            protocol.receive_login_response(*tempSocket.get());

            // 5. Si todo salió bien (no hubo excepciones), retornamos éxito
            return qMakePair(std::move(tempSocket), QString(""));

        } catch (const std::exception &e) {
            // Captura excepciones de Socket() O de ClientProtocol
            return qMakePair(nullptr, QString("Error: %1").arg(e.what()));
        }
        // --- FIN DEL HILO ---
    });
    // Asigno el futuro al watcher para que notifique cuando termine
    loginWatcher.setFuture(future);
}

// Cuando el hilo de QtConcurrent::run termina, se ejecuta esto
void LoginScreen::onLoginFinished()
{
    LoginResult result = loginWatcher.result();

    this->socket = std::move(result.first); // Muevo el socket a nuestra clase
    QString errorMsg = result.second;

    if (this->socket) {
        ui->label_error->setText("¡Conectado!");
        emit loginExitoso();

    } else {
        ui->label_error->setText(errorMsg);
        ui->btn_conectar->setEnabled(true);
    }
}

// Cuando recibo loginExitoso en MainWindow, llamo a esta funcion para "entregar" el socket
std::unique_ptr<Socket> LoginScreen::takeSocket()
{
    return std::move(this->socket);
}
