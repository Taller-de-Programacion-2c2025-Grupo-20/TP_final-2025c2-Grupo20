#include <../common/constants.h>
#include <../common/serverState.h>


class ClientReceiver {
public:
    ClientReceiver();
    
    /**
     * @brief Obtiene el último estado del servidor si está disponible en la cola.
     *
     *  Esta función NO bloquea. Si no hay estados recibidos,
     *          devuelve un ServerState por defecto.
     *
     * @return ServerState recibido desde el servidor.
     */
    ServerState pollState();
};
