#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include <string.h>

// Definições de pinos
#define LED_10 15   // Menos de 10%
#define LED_50 13   // Menos de 50%
#define LED_80 6    // Menos de 80%

#define BUTTON_1 19
#define BUTTON_2 16

#define BLOCK_SIZE 10000
#define MAX_BLOCKS 13

// Tamanho e regiões do heap
#define REGION_SIZE 20480  // 20 KB por região

static uint8_t ucHeap1[REGION_SIZE];
static uint8_t ucHeap2[REGION_SIZE];

void *pvAllocatedBlocks[MAX_BLOCKS];
int iAllocatedBlocks = 0;

// Inicializa as regiões de heap para heap_5.c
void initHeapRegions() {
    HeapRegion_t xHeapRegions[] = {
        { ucHeap1, sizeof(ucHeap1) },
        { ucHeap2, sizeof(ucHeap2) },
        { NULL, 0 } // Fim da lista
    };
    vPortDefineHeapRegions(xHeapRegions);
}

// Task de monitoramento de heap e LEDs
void TaskMonitoramento(void *pvParameters) {
    const uint32_t totalHeapSize = REGION_SIZE * 2;
    uint32_t livreHeapSize;

    while (1) {
        livreHeapSize = xPortGetFreeHeapSize();
        float percent = 100.0f * livreHeapSize / totalHeapSize;

        printf("Heap Livre: %lu bytes (%.2f%%)\n", livreHeapSize, percent);

        // Resetar LEDs
        gpio_put(LED_10, 0);
        gpio_put(LED_50, 0);
        gpio_put(LED_80, 0);

        // Acender LEDs de acordo com o uso
        if (percent <= 5.0f) {
            gpio_put(LED_10, 1);
            gpio_put(LED_50, 1);
            gpio_put(LED_80, 1);
        } else if (percent <= 10.0f) {
            gpio_put(LED_10, 1);
        } else if (percent <= 50.0f) {
            gpio_put(LED_50, 1);
        } else if (percent <= 80.0f) {
            gpio_put(LED_80, 1);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Task para alocar memória
void TaskAlocarMemoria(void *pvParameters) {
    while (1) {
        if (gpio_get(BUTTON_1) == 1) {
            if (iAllocatedBlocks < MAX_BLOCKS) {
                pvAllocatedBlocks[iAllocatedBlocks] = pvPortMalloc(BLOCK_SIZE);
                if (pvAllocatedBlocks[iAllocatedBlocks] != NULL) {
                    memset(pvAllocatedBlocks[iAllocatedBlocks], 0, BLOCK_SIZE);
                    iAllocatedBlocks++;
                    printf("Memória alocada. Blocos: %d\n", iAllocatedBlocks);
                } else {
                    printf("Falha ao alocar memória!\n");
                }
            } else {
                printf("Máximo de alocações atingido.\n");
            }

            while (gpio_get(BUTTON_1) == 1) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Task para liberar memória
void TaskLiberarMemoria(void *pvParameters) {
    while (1) {
        if (gpio_get(BUTTON_2) == 1) {
            if (iAllocatedBlocks > 0) {
                iAllocatedBlocks--;
                vPortFree(pvAllocatedBlocks[iAllocatedBlocks]);
                pvAllocatedBlocks[iAllocatedBlocks] = NULL;
                printf("Memória liberada. Blocos restantes: %d\n", iAllocatedBlocks);
            } else {
                printf("Nenhum bloco para liberar.\n");
            }

            while (gpio_get(BUTTON_2) == 1) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Função principal
int main() {
    stdio_init_all();

    // Inicializar GPIOs
    gpio_init(LED_10); gpio_set_dir(LED_10, GPIO_OUT);
    gpio_init(LED_50); gpio_set_dir(LED_50, GPIO_OUT);
    gpio_init(LED_80); gpio_set_dir(LED_80, GPIO_OUT);

    gpio_init(BUTTON_1); gpio_set_dir(BUTTON_1, GPIO_IN); gpio_pull_down(BUTTON_1);
    gpio_init(BUTTON_2); gpio_set_dir(BUTTON_2, GPIO_IN); gpio_pull_down(BUTTON_2);

    // Inicializar regiões de heap
    initHeapRegions();

    // Criar tasks
    xTaskCreate(TaskMonitoramento, "Monitoramento da Heap", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(TaskAlocarMemoria, "Alocação de Memória", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(TaskLiberarMemoria, "Liberação de Memória", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    vTaskStartScheduler();

    while (1);
    return 0;
}
