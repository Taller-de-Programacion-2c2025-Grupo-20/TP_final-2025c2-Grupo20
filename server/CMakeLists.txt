target_sources(taller_server
    PRIVATE
    
    server_main.cpp
    acceptor.cpp
    car.cpp
    collisions_listener.cpp
    client_handler.cpp
    gameloop.cpp
    queues_monitor.cpp
    receiver.cpp
    sender.cpp
    server_protocol.cpp
    server.cpp

    PUBLIC
    
    acceptor.h
    entity.h
    car.h
    collisions_listener.h
    client_handler.h
    gameloop.h
    queues_monitor.h
    receiver.h
    sender.h
    server_protocol.h
    server.h
    )
