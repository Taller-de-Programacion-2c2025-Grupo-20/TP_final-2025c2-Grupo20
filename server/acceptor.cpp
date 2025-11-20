#include "acceptor.h"
#include <utility>
#include <sys/socket.h>
#include "../common/liberror.h"

Acceptor::Acceptor(Socket& listener,
                   Queue<std::unique_ptr<ClientHandler>>& new_clients_queue) :
    listener(listener), 
    new_clients_queue(new_clients_queue) {}

void Acceptor::run() {
    uint8_t next_id = 0;
    while (should_keep_running()) {
        try {
            Socket client_skt = listener.accept();
            
            auto client_handler = std::make_unique<ClientHandler>(
                std::move(client_skt), 
                next_id
            );

            new_clients_queue.push(std::move(client_handler));
            
            next_id++;
            
        } catch (const LibError&) {
            break; 
        }
    }
}

void Acceptor::stop() {
    Thread::stop();
}