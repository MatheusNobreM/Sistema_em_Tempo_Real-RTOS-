#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>
#include "hardware/adc.h"
#include <queue.h>

#define ADC_NUM 0
#define ADC_PIN (26 + ADC_NUM)

SemaphoreHandle_t mutex;

typedef struct {
    uint16_t valor;
} potenciometro_t;

potenciometro_t dadoPot;

void potenciometro(void *pvParameters) {
    for(;;) {
        if(xSemaphoreTake(mutex, portMAX_DELAY)) {            
            dadoPot.valor = adc_read();
            printf("-- Leitura Potenciometro --\n");

            xSemaphoreGive(mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // Espera 50ms
    }
}

void printConteudo(void *pvParameters) {
    for(;;) {
        if(xSemaphoreTake(mutex, portMAX_DELAY)) {
            printf("Potenciometro: %u\n", dadoPot.valor);    
            xSemaphoreGive(mutex);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
    }
}

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(ADC_PIN);
    adc_select_input(ADC_NUM);

    mutex = xSemaphoreCreateMutex();
 
    xTaskCreate(potenciometro, "Potenciometro", 256, NULL, 2, NULL); 
    xTaskCreate(printConteudo, "PrintConteudo", 256, NULL, 1, NULL);          

    vTaskStartScheduler();

    for (;;) {}
    return 0;
}
