#include <cstdint>
#include <../common/constants.h>


class ClientSender {
public:
    ClientSender();

    /**
     * @brief Agrega un evento de input a la cola para enviar al servidor.
     * Esta función NO bloquea. Si no hay mas espacio en la cola de eventos, lo dropea.
     *      
     * @param action Indica si la tecla fue presionada o soltada.
     * @param key    Tecla afectada.
     */
    void sendEvent(InputAction action, InputKey key);          

private:

};
