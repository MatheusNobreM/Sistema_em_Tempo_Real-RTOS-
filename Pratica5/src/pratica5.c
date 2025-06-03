#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <stdio.h>

// Pin defines
#define BUTTON_PIN 15
#define LED_PIN 16

uint button_pressed = 0;

void button_task(void *pvParameters);
void led_task(void *pvParameters);
void semaphore_test(void *pvParameters);


SemaphoreHandle_t button_semaphore;
QueueHandle_t led_queue;

typedef enum {
    LED_TOGGLE 
} led_command_t;

void button_task(void *pvParameters) {
    (void)pvParameters;
    TickType_t xDelay = pdMS_TO_TICKS(50);

    while (1) {
        // Wait for semaphore
        if ((xSemaphoreTake(button_semaphore, portMAX_DELAY) == pdTRUE) && button_pressed) {
            printf("Semaphore consumed\n");
            led_command_t command = LED_TOGGLE;
            xQueueSend(led_queue, &command, portMAX_DELAY);

    
            vTaskDelay(xDelay);
            xSemaphoreGive(button_semaphore);
            printf("Semaphore Free by button task\n");
            button_pressed = !button_pressed;
        }
    }
}

// Task to blink Led
void led_task(void *pvParameters) {
    (void)pvParameters;

    led_command_t command;
    while (1) {
        
        if (xQueueReceive(led_queue, &command, portMAX_DELAY) == pdTRUE) {
            if (command == LED_TOGGLE) {
                
                gpio_put(LED_PIN, !gpio_get(LED_PIN));
            }
        }
    }
}


// Task to use semaphore
void semaphore_test(void *pvParameteres) {
	while (1) {
		if (xSemaphoreTake(button_semaphore, portMAX_DELAY) == pdTRUE) {
			printf("Semaphore Free\n");
			vTaskDelay(500);
			xSemaphoreGive(button_semaphore);
		} else {
			printf("Semaphore blocked\n");
			vTaskDelay(500);
		}
	}
}

// Button Interruption
void button_isr(uint gpio, uint32_t events) {
    static uint32_t last_interrupt_time = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time - last_interrupt_time > 200) { // 200 ms debounce
        BaseType_t higherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(button_semaphore, &higherPriorityTaskWoken);
        portYIELD_FROM_ISR(higherPriorityTaskWoken);
        button_pressed = !button_pressed;
        last_interrupt_time = current_time;
    }
}


// Button Interuption Config
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

    button_semaphore = xSemaphoreCreateBinary(); 
    led_queue = xQueueCreate(10, sizeof(led_command_t));

    xTaskCreate(semaphore_test, "Test_Semaphore", 256, NULL, 2, NULL);
    xTaskCreate(button_task, "ButtonTask", 256, NULL, 1, NULL);
    xTaskCreate(led_task,    "LedTask", 256, NULL, 1, NULL);
    
    setup_button_interrupt();

    vTaskStartScheduler();

    for (;;) {}
    return 0;
}