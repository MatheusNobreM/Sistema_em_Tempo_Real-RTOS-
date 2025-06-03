#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "pico/stdlib.h"


const uint LED_PIN = 25;


void task_blink(void *pvParameters) {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        printf("LED ON\n");
        gpio_put(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("LED OFF\n");
        gpio_put(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main() {
    stdio_init_all();
    sleep_ms(3000);
    printf("Iniciando FreeRTOS\n");
    xTaskCreate(task_blink, "LED_Task", 512, NULL, 1, NULL);
    vTaskStartScheduler();
    printf("Erro: Escalonador não iniciou!\n");
    while (1);
}