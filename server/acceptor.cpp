#include "acceptor.h"

#include <utility>

#include <sys/socket.h>

#include "../common/liberror.h"
#include "../common/socket.h"

#include "client_handler.h"

void Acceptor::run() {

    int next_id = 0;
    while (should_keep_running()) {
        try {
            Socket client_skt = skt.accept();
            Queue<ServerState>& sender_queue = clients_queues.addQueue(next_id);
            auto* c =
                    new ClientHandler(std::move(client_skt), gameloop_queue, sender_queue, next_id);
            next_id++;
            reap();
            client_handlers_list.push_back(c);
            c->start();
        } catch (const LibError&) {
            break;
        }
    }

    clear();
}

void Acceptor::stop() {
    Thread::stop();
    skt.shutdown(SHUT_RDWR);
    skt.close();
}

void Acceptor::reap() {
    client_handlers_list.remove_if([this](auto* client) {
        bool is_dead = !client->is_alive();
        if (is_dead) {
            clients_queues.markQueueForDeletion(client->client_id());
            client->stop();
            client->join();
            delete client;
        }

        return is_dead;
    });
}

void Acceptor::clear() {
    for (auto* client: client_handlers_list) {
        client->stop();
        client->join();
        delete client;
    }

    client_handlers_list.clear();
}

Acceptor::Acceptor(const char* port, Queue<InputCmd>& gameloop_queue,
                   QueuesMonitor& clients_queues):
        skt(port), gameloop_queue(gameloop_queue), clients_queues(clients_queues) {}
