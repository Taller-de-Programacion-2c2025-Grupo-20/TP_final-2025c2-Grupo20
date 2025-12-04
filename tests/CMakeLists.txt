target_sources(taller_tests
    PRIVATE
        protocol_test.cpp

        ../client/client_protocol.cpp
        ../server/server_protocol.cpp
        ../common/socket.cpp
        ../common/liberror.cpp
        ../common/resolver.cpp
        ../common/resolvererror.cpp

    PUBLIC
        ../client/client_protocol.h
        ../server/server_protocol.h
        ../common/socket.h
        ../common/constants.h
        ../common/gameState.h
        ../common/match_list.h
)

target_include_directories(taller_tests PRIVATE ../)