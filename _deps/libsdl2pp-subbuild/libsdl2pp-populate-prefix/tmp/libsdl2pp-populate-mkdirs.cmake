# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-src"
  "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-build"
  "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix"
  "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/tmp"
  "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp"
  "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src"
  "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/gero/Documents/Taller/TP3/TP_final-2025c2-Grupo20/_deps/libsdl2pp-subbuild/libsdl2pp-populate-prefix/src/libsdl2pp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
