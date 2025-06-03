#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include <stdio.h>

void led_task(void *pvParameters) {
    uint led_pin = *(uint *)pvParameters; 
    const char *task_name = pcTaskGetName(NULL);
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    while (true) {
        printf("%s: LED ON (GPIO%d)\n", task_name, led_pin);
        gpio_put(led_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
        printf("%s: LED OFF (GPIO%d)\n", task_name, led_pin);
        gpio_put(led_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
}

void led_task_once(void *pvParameters) {
    uint led_pin = *(uint *)pvParameters; // Recebe o pino GPIO
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    for (int i = 0; i < 3; i++) {
        printf("Tarefa 3: LED ON (GPIO%d)\n", led_pin);
        gpio_put(led_pin, 1);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1s delay
        printf("Tarefa 3: LED OFF (GPIO%d)\n", led_pin);
        gpio_put(led_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(1000)); // 1s delay
    }
    printf("Tarefa 3: Finalizada (GPIO%d)\n", led_pin);
    vTaskDelete(NULL); // Deleta a tarefa
}

int main() {
    stdio_init_all();
//    sleep_ms(2000); // Aguarda USB

    // Parâmetros para as tarefas
    static uint led_pin_1 = PICO_DEFAULT_LED_PIN; // GPIO25
    static uint led_pin_2 = 16; // GPIO16
    static uint led_pin_3 = 15; // GPIO21

    // Cria três tarefas com mesma prioridade
    xTaskCreate(led_task, "Tarefa 1", 256, &led_pin_3, 1, NULL); // GPIO25
    xTaskCreate(led_task, "Tarefa 2", 256, &led_pin_2, 1, NULL); // GPIO16
    xTaskCreate(led_task_once, "Tarefa 3", 256, &led_pin_1, 1, NULL); // GPIO21

    vTaskStartScheduler();
    while (1) {};
}