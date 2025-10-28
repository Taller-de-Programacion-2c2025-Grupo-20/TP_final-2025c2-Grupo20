target_sources(taller_client
  PRIVATE
    client.cpp
    client_protocol.cpp
  PUBLIC
    client.h
    client_protocol.h
)

target_include_directories(taller_client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
