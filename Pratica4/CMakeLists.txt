cmake_minimum_required(VERSION 3.12)

# Nome do projeto
set(ProjectName pratica4)

# Caminho para o FreeRTOS Kernel (modifique para seu caminho correto)
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel)

# Importa o SDK do Pico
include(pico_sdk_import.cmake)

# Importa o FreeRTOS (usando arquivo cmake próprio do port para RP2040)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Define o projeto e linguagens usadas
project(${ProjectName} C CXX ASM)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona o executável principal
add_executable(${ProjectName}
    src/pratica4.c
)

# Inclui diretórios de cabeçalho
target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${FREERTOS_KERNEL_PATH}/include
)

# Linka bibliotecas necessárias
target_link_libraries(${ProjectName}
    pico_stdlib
    FreeRTOS-Kernel-Heap4   # biblioteca heap do FreeRTOS importada no FreeRTOS_Kernel_import.cmake
    hardware_adc           # Exemplo: se usar ADC, pode linkar assim
)

# Configura o stdio para USB e UART
pico_enable_stdio_usb(${ProjectName} 1)
pico_enable_stdio_uart(${ProjectName} 0)

# Gera arquivos extras (UF2, etc)
pico_add_extra_outputs(${ProjectName})

# Caso queira modularizar ainda mais, pode colocar código em src/CMakeLists.txt
# e adicionar add_subdirectory(src) aqui, mas isso é opcional.
