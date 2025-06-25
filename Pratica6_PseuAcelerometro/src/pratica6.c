#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include "hardware/adc.h"

#define ADC_X_NUM 0        // GPIO 26
#define ADC_X_PIN (26 + ADC_X_NUM)

#define ADC_Y_NUM 1        // GPIO 27
#define ADC_Y_PIN (27 + ADC_X_NUM)

SemaphoreHandle_t mutex;

typedef struct {
    float eixo_x;  // tensão em volts
    float eixo_y;  // tensão em volts
} Potenciometro_t;

Potenciometro_t dadosPotenciometro;

void taskLerPotenciometro(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(mutex, portMAX_DELAY)) {
            adc_select_input(ADC_X_NUM);
            uint16_t leitura_x = adc_read();
            dadosPotenciometro.eixo_x = (leitura_x / 4095.0f) * 3.3f;

            if (dadosPotenciometro.eixo_x > 2.5f) {
                printf("[Ler] Tensão > 2.5V em X! Simulando trava com delay...\n");
                vTaskDelay(pdMS_TO_TICKS(5000));
            }

            adc_select_input(ADC_Y_NUM);
            uint16_t leitura_y = adc_read();
            dadosPotenciometro.eixo_y = (leitura_y / 4095.0f) * 3.3f;

            xSemaphoreGive(mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void taskMostrarPotenciometro(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(mutex, portMAX_DELAY)) {
            printf("Tensões -> X: %.2f V | Y: %.2f V\n", dadosPotenciometro.eixo_x, dadosPotenciometro.eixo_y);
            xSemaphoreGive(mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(ADC_X_PIN);
    adc_gpio_init(ADC_Y_PIN);

    mutex = xSemaphoreCreateMutex();

    xTaskCreate(taskLerPotenciometro, "Ler", 256, NULL, 2, NULL);
    xTaskCreate(taskMostrarPotenciometro, "Mostrar", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) {}
}
