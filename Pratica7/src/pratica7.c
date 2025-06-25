#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include <string.h>

#define LED_10 15   // Menos de 10%
#define LED_50 13   // Menos de 50%
#define LED_80 6   // Menos de 80%

#define BUTTON_1 19
#define BUTTON_2 16

#define FILL_BLOCK_SIZE 10000
#define MAX_BLOCKS 13

void *pvAllocatedBlocks[MAX_BLOCKS];
int iAllocatedBlocks = 0;

void TaskMonitoramento(void *pvParameters) {
    const uint32_t ulTotalHeapSize = configTOTAL_HEAP_SIZE;
    uint32_t ulFreeHeapSize;

    while (1) {
        ulFreeHeapSize = xPortGetFreeHeapSize();
        float percent = 100.0f * ulFreeHeapSize / ulTotalHeapSize;
        printf("Heap Total: %lu, Heap Livre: %lu, Porcentagem livre: %.2f%%\n", 
               ulTotalHeapSize, ulFreeHeapSize, percent);

        printf("Porcentagem livre do heap: %.2f%%\n", percent);
        // Lógica: apenas um LED aceso por vez
        gpio_put(LED_10, 0);
        gpio_put(LED_50, 0);
        gpio_put(LED_80, 0);

        // Lógica para acender os LEDs
        if (percent <= 5.0f) {
            printf("Acendendo todos os LEDs (<= 5%%)\n");
            gpio_put(LED_10, 1);
            gpio_put(LED_50, 1);
            gpio_put(LED_80, 1);
        } else if (percent <= 10.0f) {
            printf("Acendendo LED_10 (<= 10%%)\n");
            gpio_put(LED_10, 1);
        } else if (percent <= 50.0f) {
            printf("Acendendo LED_50 (<= 50%%)\n");
            gpio_put(LED_50, 1);
        } else if (percent <= 80.0f) {
            printf("Acendendo LED_80 (<= 80%%)\n");
            gpio_put(LED_80, 1);
        } else {
            printf("Nenhum LED aceso (> 80%%)\n");
        }
        // Acima de 80% todos ficam apagados

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void TaskAlocarMemoria(void *pvParameters) {
    while (1) {
        if (gpio_get(BUTTON_1) == 1) {
            if (iAllocatedBlocks < MAX_BLOCKS) {
                pvAllocatedBlocks[iAllocatedBlocks] = pvPortMalloc(FILL_BLOCK_SIZE);
                if (pvAllocatedBlocks[iAllocatedBlocks] != NULL) {
                    memset(pvAllocatedBlocks[iAllocatedBlocks], 0, FILL_BLOCK_SIZE);
                    iAllocatedBlocks++;
                    printf("Memória alocada. Blocos alocados: %d\n", iAllocatedBlocks);
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

int main() {
    stdio_init_all();

    gpio_init(LED_10); gpio_set_dir(LED_10, GPIO_OUT);
    gpio_init(LED_50); gpio_set_dir(LED_50, GPIO_OUT);
    gpio_init(LED_80); gpio_set_dir(LED_80, GPIO_OUT);

    gpio_init(BUTTON_1); gpio_set_dir(BUTTON_1, GPIO_IN); gpio_pull_down(BUTTON_1);
    gpio_init(BUTTON_2); gpio_set_dir(BUTTON_2, GPIO_IN); gpio_pull_down(BUTTON_2);

    xTaskCreate(TaskMonitoramento, "Monitoramento da Heap", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(TaskAlocarMemoria, "Alocação de Memória", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);
    xTaskCreate(TaskLiberarMemoria, "Liberação de Memória", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, NULL);

    vTaskStartScheduler();

    while (1);
    return 0;
}

