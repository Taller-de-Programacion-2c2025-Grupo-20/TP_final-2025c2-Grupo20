target_sources(taller_editor
    PRIVATE
    main.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
)


# Esto le dice al target "taller_editor" que debe
# buscar headers en el directorio de compilación
target_include_directories(taller_editor
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)
