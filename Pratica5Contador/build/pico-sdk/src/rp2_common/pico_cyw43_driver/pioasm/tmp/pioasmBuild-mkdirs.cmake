# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/pico-sdk/tools/pioasm"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pioasm"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pioasm-install"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/matheus-moreira/Documentos/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/Pratica5Contador/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
