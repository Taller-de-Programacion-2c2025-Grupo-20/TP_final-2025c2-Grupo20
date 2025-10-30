
# --- Archivos de Código (.cpp) ---
target_sources(taller_client PRIVATE
    client.cpp
    client_main.cpp
    client_protocol.cpp
    mainwindow.cpp

    Pantallas/carselectscreen.cpp
    Pantallas/lobbyscreen.cpp
    Pantallas/loginscreen.cpp
)

# --- Archivos de Cabecera (SOLO los que NO usan Q_OBJECT) ---
target_sources(taller_client PUBLIC
    client.h
    clientReceiver.h
    clientSender.h
    client_protocol.h
)

# --- Archivos de Interfaz (.ui) ---
target_sources(taller_client PRIVATE
    mainwindow.ui

    Pantallas/carselectscreen.ui
    Pantallas/lobbyscreen.ui
    Pantallas/loginscreen.ui
)

# --- Directorios de Inclusión ---
target_include_directories(taller_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Pantallas
)
