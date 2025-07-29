#include "pico/stdlib.h"

// === Pinos usados ===
#define BOTAO_GPIO   18   // GPIO18 → botão com pull-up
#define L298_IN1     17   // GPIO17 → IN1 do L298N
#define L298_IN2     16   // GPIO16 → IN2 do L298N

// === Estado da bomba ===
bool bomba_ativa = false;

// === Inicializa bomba (desligada) ===
void bomba_init() {
    gpio_init(L298_IN1);
    gpio_set_dir(L298_IN1, GPIO_OUT);
    gpio_put(L298_IN1, 0);

    gpio_init(L298_IN2);
    gpio_set_dir(L298_IN2, GPIO_OUT);
    gpio_put(L298_IN2, 0);
}

// === Liga a bomba (IN1 HIGH, IN2 LOW) ===
void bomba_on() {
    gpio_put(L298_IN1, 1);
    gpio_put(L298_IN2, 0);
}

// === Desliga a bomba (ambos LOW) ===
void bomba_off() {
    gpio_put(L298_IN1, 0);
    gpio_put(L298_IN2, 0);
}

int main() {
    stdio_init_all();

    // Inicializa pinos
    bomba_init();

    gpio_init(BOTAO_GPIO);
    gpio_set_dir(BOTAO_GPIO, GPIO_IN);
    gpio_pull_up(BOTAO_GPIO); // usa pull-up interno

    bool estado_anterior = 1;

    while (1) {
        bool estado_atual = gpio_get(BOTAO_GPIO);

        // Detecta borda de descida (botão pressionado)
        if (estado_anterior == 1 && estado_atual == 0) {
            // Inverte estado da bomba
            bomba_ativa = !bomba_ativa;
            if (bomba_ativa) {
                bomba_on();
                printf("Bomba LIGADA\n");
            } else {
                bomba_off();
                printf("Bomba DESLIGADA\n");
            }

            sleep_ms(300); // debounce simples
        }

        estado_anterior = estado_atual;
        sleep_ms(10); // pequena pausa para evitar loop rápido
    }
}
