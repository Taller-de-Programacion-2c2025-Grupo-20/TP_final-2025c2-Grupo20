#include "client_sender.h"

#include "../common/liberror.h"
#include <QDebug>

void ClientSender::run() {

    while (should_keep_running()) {
        try {

            InputCmd response = client_queue.pop();
            qDebug() << "CLIENT DEBUG 3: 'Sender' sacó un comando de la cola. Key:" << (int)response.key;
            switch (response.key) {
                
                case InputKey::Up:
                case InputKey::Down:
                case InputKey::Left:
                case InputKey::Right:
                    protocol.send_input(response);
                    break;

                case InputKey::CreateMatch:
                    qDebug() << "CLIENT DEBUG 4: Dispatching CreateMatch...";
                    protocol.send_create_match(response.username);
                    break;
                
                case InputKey::JoinMatch:
                    protocol.send_join_match(response.match_id);
                    break;

                case InputKey::StartGame:
                    protocol.send_start_game();
                    break;
                case InputKey::Quit:
                    break;
                default:
                    break;
            }
        } catch (const ClosedQueue&) {
            break;
        } catch (const LibError& e) {
            break;
        }
    }
}

void ClientSender::stop() {
    Thread::stop();
    client_queue.close();
}

ClientSender::ClientSender(ClientProtocol& protocol, Queue<InputCmd>& client_queue):
        protocol(protocol), client_queue(client_queue) {}
