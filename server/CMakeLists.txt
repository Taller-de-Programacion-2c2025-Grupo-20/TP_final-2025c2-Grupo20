target_sources(taller_server
    PRIVATE
    
    server_main.cpp
    acceptor.cpp
    
    world_entities/car.cpp
    world_entities/wall.cpp
    world_entities/checkpoint.cpp
    
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
    
    world_entities/entity.h
    world_entities/car.h
    world_entities/wall.h
    world_entities/checkpoint.h
    
    collisions_listener.h
    client_handler.h
    gameloop.h
    queues_monitor.h
    receiver.h
    sender.h
    server_protocol.h
    server.h
    match.h
    match.cpp
    )
