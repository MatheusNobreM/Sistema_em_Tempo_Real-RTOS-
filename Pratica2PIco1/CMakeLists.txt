cmake_minimum_required(VERSION 3.12)

# Name project
SET(ProjectName pratica3Pico1)

# Set any variables required for importing libraries
SET(FREERTOS_KERNEL_PATH /home/matheus-moreira/Faculdade/2025.1/Rtos/Sistema_em_Tempo_Real-RTOS-/FreeRTOS-Kernel)

# Import those libraries
include(pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2350/FreeRTOS_Kernel_import.cmake)

# Define project
project(${ProjectName})

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Add definitions for RP2350
add_definitions(-DPICO_RP2350=1)

# Add subdirectories
add_subdirectory(src)

# if you have anything in "lib" folder then uncomment below - remember to add a CMakeLists.txt
# file to the "lib" directory
#add_subdirectory(lib)