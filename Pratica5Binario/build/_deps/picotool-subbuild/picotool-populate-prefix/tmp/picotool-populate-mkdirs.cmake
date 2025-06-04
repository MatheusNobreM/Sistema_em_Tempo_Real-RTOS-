# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-src"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-build"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-subbuild/picotool-populate-prefix"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-subbuild/picotool-populate-prefix/tmp"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-subbuild/picotool-populate-prefix/src"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Binario/build/_deps/picotool-subbuild/picotool-populate-prefix/src/picotool-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
