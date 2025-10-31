#ifndef CLIENT_RECEIVER_H
#define CLIENT_RECEIVER_H

#include <mutex>
#include <atomic>
#include "../common/thread.h"
#include "../common/queue.h"
#include "../common/gameState.h"
#include "client_protocol.h"

class ClientReceiver : public Thread {
private:
    ClientProtocol& protocol;
    
    Queue<GameStateDTO>& state_queue;
    
    /**
     * @brief Loop principal del hilo.
     * Lee 1 byte (comando) y decide cómo leer el resto del paquete.
     */
    virtual void run() override;

public:
    ClientReceiver(ClientProtocol& protocol, Queue<GameStateDTO>& state_queue);

    /**
     * @brief Detiene el hilo de forma segura.
     */
    void stop() override;
};

#endif