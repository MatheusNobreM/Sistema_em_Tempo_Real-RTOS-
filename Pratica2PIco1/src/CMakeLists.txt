add_executable(pratica2Pico1
    main.c
)
target_include_directories(pratica2Pico1 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/include
)
target_link_libraries(pratica2Pico1
    pico_stdlib
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)
pico_enable_stdio_usb(pratica2Pico1 1)
pico_enable_stdio_uart(pratica2Pico1 0)
pico_add_extra_outputs(pratica2Pico1)