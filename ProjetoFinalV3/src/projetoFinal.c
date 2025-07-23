#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/adc.h"
#include <stdio.h>

// --- pinos analógicos ---
#define KY028_ANALOG_PIN 26   // GPIO26 → ADC0
#define MQ2_ANALOG_PIN   27   // GPIO27 → ADC1

// --- parâmetros dos sensores ---
#define RL_VALUE         10000.0f  // 10kΩ para o MQ-2
#define VCC_MQ2          5.0f      // tensão de alimentação do MQ-2

// --- pinos do driver L298A ---
#define L298_IN1_PIN     14   // GPIO14 → IN1
#define L298_IN2_PIN     15   // GPIO15 → IN2

// --- limiar de temperatura ---
#define TEMP_THRESHOLD   30.0f  // °C: acima disso, liga a bomba

// Estima temperatura a partir da tensão no KY-028
float estimar_temperatura(float tensao) {
    return -100.0f * tensao + 235.0f;
}

// Calcula resistência Rs do MQ-2
float calcular_rs(float vrl, float vc) {
    if (vrl <= 0.01f) return 0.0f;  // evita divisão por zero
    return (vc - vrl) * RL_VALUE / vrl;
}

// Inicializa o driver L298A para controle digital da bomba
void pump_init() {
    gpio_init(L298_IN1_PIN);
    gpio_set_dir(L298_IN1_PIN, GPIO_OUT);
    gpio_init(L298_IN2_PIN);
    gpio_set_dir(L298_IN2_PIN, GPIO_OUT);
    // ATENÇÃO: conectar GND da fonte externa ao GND do RP2040
}

// Liga a bomba (IN1 HIGH, IN2 LOW)
void pump_on() {
    gpio_put(L298_IN1_PIN, 1);
    gpio_put(L298_IN2_PIN, 0);
}

// Desliga a bomba (IN1 LOW, IN2 LOW)
void pump_off() {
    gpio_put(L298_IN1_PIN, 0);
    gpio_put(L298_IN2_PIN, 0);
}

// Tarefa de leitura do KY-028 e controle da bomba
void task_read_temperature(void *params) {
    adc_gpio_init(KY028_ANALOG_PIN);
    while (1) {
        adc_select_input(0);  // ADC0 = GPIO26
        uint16_t leitura_adc = adc_read();
        float tensao = (leitura_adc / 4095.0f) * 3.3f;
        float temperatura = estimar_temperatura(tensao);

        printf("[KY-028] ADC: %u  Tensão: %.2f V  Temp: %.1f °C\n",
               leitura_adc, tensao, temperatura);

        if (temperatura >= TEMP_THRESHOLD) {
            pump_on();
            printf("[Pump] ON\n");
        } else {
            pump_off();
            printf("[Pump] OFF\n");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Tarefa de leitura do MQ-2
void task_read_mq2(void *params) {
    adc_gpio_init(MQ2_ANALOG_PIN);
    while (1) {
        adc_select_input(1);  // ADC1 = GPIO27
        uint16_t leitura_adc = adc_read();
        float vrl = (leitura_adc / 4095.0f) * 3.3f;
        float rs = calcular_rs(vrl, VCC_MQ2);

        printf("[MQ-2] ADC: %u  VRL: %.2f V  Rs: %.1f Ω\n",
               leitura_adc, vrl, rs);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // aguarda terminal serial

    adc_init();      // inicializa ADC
    pump_init();     // inicializa driver L298A

    xTaskCreate(task_read_temperature, "ReadTemp", 256, NULL, 1, NULL);
    xTaskCreate(task_read_mq2,         "ReadMQ2",  256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (1) {
        // não deve chegar aqui
    }
}
