#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include <stdio.h>

void led_task(void *pvParameters) {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN; // GPIO25
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        printf("Tarefa 1: LED ON (GPIO25)\n");
        gpio_put(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        printf("Tarefa 1: LED OFF (GPIO25)\n");
        gpio_put(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void led_task2(void *pvParameters) {
    const uint LED_PIN2 = 16; // GPIO16
    gpio_init(LED_PIN2);
    gpio_set_dir(LED_PIN2, GPIO_OUT);
    while (true) {
        printf("Tarefa 2: LED ON (GPIO16)\n");
        gpio_put(LED_PIN2, 1);
        vTaskDelay(pdMS_TO_TICKS(2000));
        printf("Tarefa 2: LED OFF (GPIO16)\n");
        gpio_put(LED_PIN2, 0);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Aguarda 2s para USB
    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(led_task2, "LED_Task2", 256, NULL, 2, NULL);
    vTaskStartScheduler();
    while (1) {};
}