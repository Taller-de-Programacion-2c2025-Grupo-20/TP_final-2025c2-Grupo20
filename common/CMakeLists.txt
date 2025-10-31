target_sources(taller_common
    PRIVATE
    
    liberror.cpp
    resolver.cpp
    resolvererror.cpp
    socket.cpp

    PUBLIC
    
    clientCommand.h
    constants.h
    liberror.h
    queue.h
    serverState.h
    resolver.h
    resolvererror.h
    socket.h
    thread.h
    gameState.h

    )
