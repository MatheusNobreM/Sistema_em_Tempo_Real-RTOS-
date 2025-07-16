#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/adc.h"

#define KY028_ANALOG_PIN 26 // GPIO26 - ADC0
#define MQ2_ANALOG_PIN   27 // GPIO27 - ADC1

#define RL_VALUE 10000.0f   // 10k Ohm para o MQ-2
#define VCC_MQ2   5.0f      // Tensão de alimentação do MQ-2 (ajuste se for diferente)

// Função para estimar temperatura do KY-028
float estimar_temperatura(float tensao) {
    // Exemplo: 2.10V ≈ 25°C e 2.00V ≈ 35°C
    return -100.0f * tensao + 235.0f;
}

// Função para calcular resistência Rs do MQ-2
float calcular_rs(float vrl, float vc) {
    if (vrl <= 0.01f) return 0.0f; // evitar divisão por zero
    return (vc - vrl) * RL_VALUE / vrl;
}

// Tarefa para ler o sensor KY-028
void task_read_temperature(void *params) {
    adc_gpio_init(KY028_ANALOG_PIN);
    while (1) {
        adc_select_input(0); // ADC0 = GPIO26
        uint16_t leitura_adc = adc_read();
        float tensao = (leitura_adc / 4095.0f) * 3.3f;
        float temperatura = estimar_temperatura(tensao);

        printf("[KY-028] ADC: %u\tTensão: %.2f V\tTemp estimada: %.1f °C\n",
               leitura_adc, tensao, temperatura);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Tarefa para ler o sensor MQ-2
void task_read_mq2(void *params) {
    adc_gpio_init(MQ2_ANALOG_PIN);
    while (1) {
        adc_select_input(1); // ADC1 = GPIO27
        uint16_t leitura_adc = adc_read();
        float vrl = (leitura_adc / 4095.0f) * 3.3f; // VRL lido pelo Pico
        float rs = calcular_rs(vrl, VCC_MQ2);

        printf("[MQ-2] ADC: %u\tVRL: %.2f V\tRs: %.1f Ohms\n",
               leitura_adc, vrl, rs);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000); // Aguarda terminal serial

    adc_init(); // Inicializa o ADC

    // Criação das tarefas
    xTaskCreate(task_read_temperature, "ReadTemp", 256, NULL, 1, NULL);
    xTaskCreate(task_read_mq2, "ReadMQ2", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {}
}
