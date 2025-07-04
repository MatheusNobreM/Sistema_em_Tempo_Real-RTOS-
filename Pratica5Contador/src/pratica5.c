#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <stdio.h>

#define BUTTON_PIN 15
#define LED_PIN 16

SemaphoreHandle_t semaphone;
QueueHandle_t queue;

uint click = 0;
typedef enum {
    LED_TOGGLE 
} led_command_t;

void button(void *pvParameters) {
    while (1) {
        if ((xSemaphoreTake(semaphone, portMAX_DELAY) == pdTRUE) && click) {
            printf("Semaforo consumido pela task do botão\n");
            led_command_t command = LED_TOGGLE;
            xQueueSend(queue, &command, portMAX_DELAY);

            vTaskDelay(300); // para visualização
            xSemaphoreGive(semaphone);
            printf("Semaforo liberado pela task do botão\n");
            click = !click;
        }
    }
}

void led(void *pvParameters) {
    led_command_t command;
    while (1) {
        if (xQueueReceive(queue, &command, portMAX_DELAY) == pdTRUE) {//xSemaphoreTake(semaphone, pdMS_TO_TICKS(100))
            if (command == LED_TOGGLE) {
                gpio_put(LED_PIN, !gpio_get(LED_PIN));
            }
        }
    }
}

void semaphone_task(void *pvParameteres) {
	while (1) {
		if (xSemaphoreTake(semaphone, portMAX_DELAY) == pdTRUE) {
            printf("Livre\n");
			vTaskDelay(300); // para visualização
			xSemaphoreGive(semaphone);
		} else {
			printf("Bloqueado\n");
			vTaskDelay(300); // para visualização
		}
	}
}

void button_isr(uint gpio, uint32_t events) {
    static uint32_t last_interrupt_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time - last_interrupt_time > 300) { // 300 ms debounce
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        xSemaphoreGive(semaphone, &higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
        click = !click;
        last_interrupt_time = current_time;
    }
}

void setup_button_interrupt(void) {
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, button_isr);
}

int main() {
    stdio_init_all();
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN); 
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    semaphone = xSemaphoreCreateCounting(3, 0); // Capacidade do semáforo: 3
    
    queue = xQueueCreate(10, sizeof(led_command_t));

    xTaskCreate(button, "ButtonTask", 256, NULL, 1, NULL);
    xTaskCreate(led, "LedTask", 256, NULL, 1, NULL);
    xTaskCreate(semaphone_task, "SemaforoTask", 256, NULL, 2, NULL);

    setup_button_interrupt();
    vTaskStartScheduler();

   
    for (;;) {}
    return 0;
}