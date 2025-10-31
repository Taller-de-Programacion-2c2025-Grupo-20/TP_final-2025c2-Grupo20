#include "receiver.h"
#include "../common/constants.h"
#include <stdexcept>
#include "client_protocol.h"

Receiver::Receiver(ClientProtocol& protocol) :
    protocol(protocol),
    state_queue(1)
{
    last_known_state.car_count = 0;
}

void Receiver::run() {
    try {
        while (should_keep_running()) {
            
            uint8_t cmd_code;
            switch (cmd_code) {
                case EVT_GAME_STATE: { 
                    GameStateDTO new_state = protocol.receive_game_state();
                    GameStateDTO dummy;
                    state_queue.try_pop(dummy); 
                    state_queue.push(std::move(new_state));
                    break;
                }
                case EVT_RACE_FINISHED: { 
                    // RaceResults results = protocol.receive_race_results();
                    // race_finished = true;
                    break;
                }
                default:
                    throw std::runtime_error("Error: Comando de servidor desconocido.");
                    break;
            }
        }
    } catch (const std::exception& e) {
        if (should_keep_running()) {
            std::cerr << "Error en Receiver: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Obtiene el último estado del juego (no bloqueante).
 * Llamado por el hilo principal (gráficos) en cada fotograma.
 */
GameStateDTO Receiver::pollState() {
    GameStateDTO new_state;

    if (state_queue.try_pop(new_state)) {
        std::lock_guard<std::mutex> lock(state_mtx);
        last_known_state = new_state;
        return new_state;
    } else {
        std::lock_guard<std::mutex> lock(state_mtx);
        return last_known_state;
    }
}