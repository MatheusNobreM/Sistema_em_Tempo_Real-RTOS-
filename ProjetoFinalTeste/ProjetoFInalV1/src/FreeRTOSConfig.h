#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Configurações básicas do FreeRTOS */
#define configENABLE_MPU 0
#define configENABLE_FPU          1
#define configENABLE_TRUSTZONE    0
#define configUSE_PREEMPTION 1
#define configUSE_TICKLESS_IDLE 0
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0
#define configUSE_MALLOC_FAILED_HOOK 0

#define configTICK_RATE_HZ ((TickType_t)1000)         // Taxa do tick: 1000 Hz (1ms)
#define configMAX_PRIORITIES 32                        // Prioridades máximas
#define configMINIMAL_STACK_SIZE ((configSTACK_DEPTH_TYPE)256)  // Tamanho mínimo da pilha
#define configTOTAL_HEAP_SIZE (128*1024)               // Heap total: 128 KB
#define configCPU_CLOCK_HZ (125000000)                  // Frequência CPU RP2350 125 MHz
#define configUSE_16_BIT_TICKS 0
#define configIDLE_SHOULD_YIELD 1

/* Sincronização */
#define configUSE_MUTEXES 1
#define configUSE_RECURSIVE_MUTEXES 1
#define configUSE_COUNTING_SEMAPHORES 1

#define configQUEUE_REGISTRY_SIZE 8
#define configUSE_QUEUE_SETS 1

#define configUSE_TIME_SLICING 1

/* Timers */
#define configUSE_TIMERS 1
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH 10
#define configTIMER_TASK_STACK_DEPTH 1024

/* Tipos e alocações */
#define configSTACK_DEPTH_TYPE uint32_t
#define configSUPPORT_DYNAMIC_ALLOCATION 1

/* Prioridades de interrupção */
#define configKERNEL_INTERRUPT_PRIORITY 255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191

/* Assert */
#include <assert.h>
#define configASSERT(x) assert(x)

/* Inclusão das APIs do FreeRTOS */
#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_xTaskGetCurrentTaskHandle 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_xTaskGetIdleTaskHandle 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTimerPendFunctionCall 1
#define INCLUDE_xTaskAbortDelay 1
#define INCLUDE_xTaskGetHandle 1
#define INCLUDE_xTaskResumeFromISR 1
#define INCLUDE_xQueueGetMutexHolder 1

#endif /* FREERTOS_CONFIG_H */
