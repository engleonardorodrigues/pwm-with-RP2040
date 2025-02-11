/*****************************************************************************************************
 * Título        : Controle de Servomotor por PWM para Raspberry Pi Pico W
 * Desenvolvedor : Leonardo Rodrigues
 * Versão        : 1.0.0
 * 
 * Descrição:
 * Este programa implementa um sistema de controle de ângulo para um servomotor padrão utilizando
 * o Raspberry Pi Pico W. O sinal PWM é configurado para posicionar o servo em diferentes ângulos
 * (0°, 90° e 180°) com esperas de 5 segundos, além de realizar uma varredura suave entre 0° e 180°.
 * Um LED RGB (na GPIO 12) também pode ser observado para verificar o comportamento durante o 
 * funcionamento do servo.
 * 
 * Materiais utilizados:
 * 
 * 1 - Raspberry Pi Pico W
 * 1 - Servomotor (micro servo padrão)
 * 1 - LED RGB (GPIO 12)
 * 1 - Simulador de eletrônica Wokwi
 *****************************************************************************************************/ 


#include <stdio.h>                                                                              // Biblioteca padrão do C para funções de entrada e saída
#include "pico/stdlib.h"                                                                        // Biblioteca para configurações e funções padrão do Raspberry Pi Pico
#include "hardware/pwm.h"                                                                       // Biblioteca específica para controle de PWM no RP2040

#define SERVO_PIN        22                                                                     // Define o GPIO 22 como saída de sinal para o servomotor
#define LED_RGB_PIN      12                                                                     // Define o GPIO 12 como saída para o sinal do LED RGB

#define PWM_FREQUENCY_HZ 50                                                                     // Define a frequência desejada de 50 Hz (período de ~20 ms)
#define PERIOD_US        20000                                                                  // Período de 20.000 microssegundos (20 ms)

#define SERVO_MIN_US     500                                                                    // Largura de pulso de 500 us (~0 graus no servo)
#define SERVO_MID_US     1470                                                                   // Largura de pulso de 1470 us (~90 graus no servo)
#define SERVO_MAX_US     2400                                                                   // Largura de pulso de 2400 us (~180 graus no servo)

#define STEP_US          5                                                                      // Incremento de 5 us para a varredura suave
#define DELAY_MS         10                                                                     // Atraso de 10 ms entre cada incremento de pulso

// Função para converter tempo de pulso (em microsegundos) em nível de PWM (0 a PERIOD_US-1)
static inline uint16_t pulse_to_level(uint32_t pulse_us) {                                      // Define a função como "inline" para otimizar chamadas
    if (pulse_us > PERIOD_US) {                                                                 // Se o valor de pulso for maior que o período total
        pulse_us = PERIOD_US;                                                                   // Ajusta para o máximo permitido (periodo)
    }
    return (uint16_t) pulse_us;                                                                 // Retorna diretamente o valor que será usado como nível de PWM
}

int main() {                                                                                    // Função principal do programa
    stdio_init_all();                                                                           // Inicializa entrada/saída padrão (utilizada para debug)

    // Configuração do PWM para o servo no GPIO 22
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);                                                // Configura o pino 22 para a função de PWM
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);                                          // Obtém o número do slice de PWM vinculado ao pino 22
    uint channel = pwm_gpio_to_channel(SERVO_PIN);                                              // Obtém o canal (A ou B) associado ao pino

    pwm_set_wrap(slice_num, PERIOD_US - 1);                                                     // Configura o contador do PWM para "encerrar" em 19999 (0 a 19999 total = 20000 ticks)
    pwm_set_clkdiv(slice_num, 125.0f);                                                          // Define o divisor de clock: 125 MHz / 125 = 1 MHz → cada tick = 1 us

    // Inicia o servo em ~0 graus (pulso de 500 us)
    pwm_set_chan_level(slice_num, channel, pulse_to_level(SERVO_MIN_US));                       // Define o nível do canal para 500 us
    pwm_set_enabled(slice_num, true);                                                           // Habilita o slice PWM

    // Configuração do LED RGB no GPIO 12 como saída
    gpio_init(LED_RGB_PIN);                                                                     // Inicializa o pino 12
    gpio_set_dir(LED_RGB_PIN, GPIO_OUT);                                                        // Define o pino 12 como saída
    bool led_state = false;                                                                     // Cria uma variável para armazenar o estado do LED (ligado/desligado)

    // 1) Servo em ~180 graus por 5s
    pwm_set_chan_level(slice_num, channel, pulse_to_level(SERVO_MAX_US));                       // Ajusta pulso para 2400 us (~180 graus)
    sleep_ms(5000);                                                                             // Aguarda 5 segundos

    // 2) Servo em ~90 graus por 5s
    pwm_set_chan_level(slice_num, channel, pulse_to_level(SERVO_MID_US));                       // Ajusta pulso para 1470 us (~90 graus)
    sleep_ms(5000);                                                                             // Aguarda 5 segundos

    // 3) Servo em ~0 graus por 5s
    pwm_set_chan_level(slice_num, channel, pulse_to_level(SERVO_MIN_US));                       // Ajusta pulso para 500 us (~0 graus)
    sleep_ms(5000);                                                                             // Aguarda 5 segundos

    // 4) Varredura suave entre ~0 graus e ~180 graus
    while (true) {                                                                              // Loop infinito
        // 4.1) Varredura do mínimo (500 us) até o máximo (2400 us)
        for (uint32_t pulse = SERVO_MIN_US; pulse <= SERVO_MAX_US; pulse += STEP_US) {          // Incrementa o pulso de 5 em 5 us
            pwm_set_chan_level(slice_num, channel, pulse_to_level(pulse));                      // Ajusta o nível PWM conforme o valor do pulso
            sleep_ms(DELAY_MS);                                                                 // Aguarda 10 ms (suaviza o movimento)

            led_state = !led_state;                                                             // Alterna o estado do LED
            gpio_put(LED_RGB_PIN, led_state);                                                   // Atualiza o LED conforme o novo estado
        }

        // 4.2) Retorno do máximo (2400 us) até o mínimo (500 us)
        for (int32_t pulse = SERVO_MAX_US; pulse >= (int32_t)SERVO_MIN_US; pulse -= STEP_US) {  // Decrementa o pulso de 5 em 5 us
            pwm_set_chan_level(slice_num, channel, pulse_to_level(pulse));                      // Ajusta o nível PWM conforme o valor do pulso
            sleep_ms(DELAY_MS);                                                                 // Aguarda 10 ms (suaviza o movimento)

            led_state = !led_state;                                                             // Alterna o estado do LED
            gpio_put(LED_RGB_PIN, led_state);                                                   // Atualiza o LED conforme o novo estado
        }
    }

    return 0;                                                                                   
}
