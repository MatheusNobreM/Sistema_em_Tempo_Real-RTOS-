cmake_minimum_required(VERSION 3.12)
SET(ProjectName pratica2)
SET(FREERTOS_KERNEL_PATH /home/matheus-moreira/Faculdade/2025.1/Rtos/pico-sdk/lib/FreeRTOS-Kernel)
include(/home/matheus-moreira/Faculdade/2025.1/Rtos/pico-sdk/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)
project(${ProjectName} C CXX ASM)
pico_sdk_init()
add_subdirectory(src)
include_directories(${FREERTOS_KERNEL_PATH}/include)
include_directories(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include)