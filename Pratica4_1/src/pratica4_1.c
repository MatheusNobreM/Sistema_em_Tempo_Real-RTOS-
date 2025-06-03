#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LEDS_ON 1
#define LEDS_OFF 0
#define LEDS_YELLOW 17    
#define BUTTON_GPIO 15  

typedef struct {
    unsigned int num;
    unsigned int status;
} LedInfo;

QueueHandle_t ledQueue; // apenas uma instância global aqui

void drvLedsSet(unsigned int led_num, unsigned int led_status) {
    gpio_put(led_num, (led_status == LEDS_ON) ? 1 : 0);
}

void init_led_gpio(void) {
    gpio_init(LEDS_YELLOW);
    gpio_set_dir(LEDS_YELLOW, GPIO_OUT);
    gpio_put(LEDS_YELLOW, 0); // apaga o LED inicialmente
}

void gpio_callback(uint gpio, uint32_t events) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    LedInfo led;

    led.num = LEDS_YELLOW;
    led.status = (gpio_get(gpio) == 0) ? LEDS_ON : LEDS_OFF;  // botão pressionado = nível baixo

    xQueueSendToBackFromISR(ledQueue, &led, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void init_button_gpio(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio); 
    gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, gpio_callback);
}

void blink_led_1(void *pvParameters) {
    LedInfo led;

    for (;;) {
        if (xQueueReceive(ledQueue, &led, portMAX_DELAY)) {
            drvLedsSet(led.num, led.status);
        }
    }
}

int main(void) {
    stdio_init_all();

    init_led_gpio();
    init_button_gpio(BUTTON_GPIO);

    ledQueue = xQueueCreate(10, sizeof(LedInfo));
    if (ledQueue == NULL) {
        printf("Erro ao criar a fila!\n");
        while (1);
    }

    xTaskCreate(blink_led_1, "TaskLed", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();

    while (1);  // não deve chegar aqui
}
