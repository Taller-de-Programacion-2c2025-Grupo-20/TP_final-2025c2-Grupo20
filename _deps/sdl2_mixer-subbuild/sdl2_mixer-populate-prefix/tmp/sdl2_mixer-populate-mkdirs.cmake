# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-src"
  "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-build"
  "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-subbuild/sdl2_mixer-populate-prefix"
  "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-subbuild/sdl2_mixer-populate-prefix/tmp"
  "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-subbuild/sdl2_mixer-populate-prefix/src/sdl2_mixer-populate-stamp"
  "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-subbuild/sdl2_mixer-populate-prefix/src"
  "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-subbuild/sdl2_mixer-populate-prefix/src/sdl2_mixer-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-subbuild/sdl2_mixer-populate-prefix/src/sdl2_mixer-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/nacho/Escritorio/TALLER/TP_final-2025c2-Grupo20/_deps/sdl2_mixer-subbuild/sdl2_mixer-populate-prefix/src/sdl2_mixer-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
