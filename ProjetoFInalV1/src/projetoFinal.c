#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/adc.h"

#define KY028_ANALOG_PIN 26 // GPIO26 - ADC0

// Função de estimativa de temperatura com base na tensão (calibrada por você)
float estimar_temperatura(float tensao) {
    // Baseado na aproximação: 2.10V ≈ 25°C e 2.00V ≈ 35°C
    return -100.0f * tensao + 235.0f;
}

// Tarefa para ler o sensor KY-028 e imprimir no terminal
void task_read_temperature(void *params) {
    adc_init();
    adc_gpio_init(KY028_ANALOG_PIN);
    adc_select_input(0); // GPIO26 = ADC0

    while (1) {
        uint16_t leitura_adc = adc_read();
        float tensao = (leitura_adc / 4095.0f) * 3.3f;
        float temperatura = estimar_temperatura(tensao);

        printf("Leitura ADC: %u\tTensão: %.2f V\tTemperatura estimada: %.1f °C\n",
               leitura_adc, tensao, temperatura);

        vTaskDelay(pdMS_TO_TICKS(1000)); // a cada 1 segundo
    }
}

int main() {
    stdio_init_all(); 

    // Aguarda conexão com terminal serial (opcional, evita perda dos primeiros prints)
    sleep_ms(2000);

    xTaskCreate(task_read_temperature, "ReadTemp", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
}
