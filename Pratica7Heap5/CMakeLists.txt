cmake_minimum_required(VERSION 3.12)

# Nome do projeto
set(ProjectName pratica7)

# Caminho para o FreeRTOS Kernel
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel)

# Importa o SDK do Pico
include(pico_sdk_import.cmake)

# Importa o FreeRTOS para RP2040
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define o projeto
project(${ProjectName} C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável
add_executable(${ProjectName}
    src/pratica7.c
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_5.c
)

# Inclui diretórios de cabeçalhos
target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040
    ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/portable  # <- este resolve o erro do portmacro.h
)

# Linka bibliotecas necessárias
target_link_libraries(${ProjectName}
    pico_stdlib
    hardware_adc
)

# Configura USB/UART
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

# Gera UF2, etc.
pico_add_extra_outputs(${ProjectName})
