#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/adc.h"

// Pinos
#define MQ2_PIN       26    // ADC0 (GPIO26)
#define RELE_PIN      16    // GPIO para módulo relé
#define LED_ALERTA    17    // GPIO para LED

// Limite de fumaça (ajustável)
#define LIMIAR_FUMACA 1000  // valor ADC (0-4095)

// Variável compartilhada
volatile bool fire_detected = false;

// Tarefa de detecção de fumaça
void task_detect_smoke(void *params) {
    adc_init();
    adc_gpio_init(MQ2_PIN);
    adc_select_input(0); // ADC0 para GPIO26

    while (1) {
        uint16_t leitura = adc_read();
        if (leitura > LIMIAR_FUMACA) {
            fire_detected = true;
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // 100 ms
    }
}

// Tarefa de controle (bomba e LED)
void task_control(void *params) {
    gpio_init(RELE_PIN);
    gpio_set_dir(RELE_PIN, GPIO_OUT);
    gpio_put(RELE_PIN, 0); // bomba desligada

    gpio_init(LED_ALERTA);
    gpio_set_dir(LED_ALERTA, GPIO_OUT);
    gpio_put(LED_ALERTA, 0); // LED desligado

    while (1) {
        if (fire_detected) {
            gpio_put(RELE_PIN, 1);     // Liga bomba
            gpio_put(LED_ALERTA, 1);   // Liga LED

            vTaskDelay(pdMS_TO_TICKS(5000)); // espera 5s

            gpio_put(RELE_PIN, 0);     // Desliga bomba
            gpio_put(LED_ALERTA, 0);   // Desliga LED

            fire_detected = false;     // Reinicia estado
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // espera curta
    }
}

int main() {
    stdio_init_all();

    xTaskCreate(task_detect_smoke, "Detect Smoke", 256, NULL, 1, NULL);
    xTaskCreate(task_control,     "Control Pump", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true) {
        // nunca deve chegar aqui
    }
}
