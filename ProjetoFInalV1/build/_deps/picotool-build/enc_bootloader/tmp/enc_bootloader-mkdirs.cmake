# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-src/enc_bootloader"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader/tmp"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader/src"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/ProjetoFInalV1/build/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
