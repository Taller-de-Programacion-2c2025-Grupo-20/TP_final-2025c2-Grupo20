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
    
    Queue<GameStateDTO> state_queue;
    
    GameStateDTO last_known_state;
    std::mutex state_mtx;
    /**
     * @brief Loop principal del hilo.
     * Lee 1 byte (comando) y decide cómo leer el resto del paquete.
     */
    virtual void run() override;

public:
    ClientReceiver(ClientProtocol& protocol);
    
    /**
     * @brief Obtiene el último estado del juego (no bloqueante).
     */
    GameStateDTO pollState();

    /**
     * @brief Detiene el hilo de forma segura.
     */
    void stop();
};

#endif