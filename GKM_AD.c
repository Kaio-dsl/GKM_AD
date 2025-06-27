#include <stdio.h>
#include <stdlib.h>     
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// --- DEFINIÇÕES E CONSTANTES GLOBAIS ---

// Pinos de hardware
#define PINO_JOYSTICK_Y   26
#define PINO_SERVO        20
#define PINO_LED          13

// Parâmetros do ADC e Joystick
#define VALOR_MAX_ADC       4095
#define PONTO_CENTRAL_ADC   (VALOR_MAX_ADC / 2)
#define ZONA_MORTA_JOYSTICK 150      // Tolerância para o joystick em repouso

// Parâmetros do Servo Motor
#define PULSO_MINIMO_US     500      // Pulso em microssegundos para a posição de 0 graus
#define PULSO_MAXIMO_US     2480     // Pulso em microssegundos para a posição de 180 graus

// Parâmetros do PWM do LED
#define WRAP_LED            255      // Nível máximo de brilho do LED (faixa de 0-255)


// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE ATUALIZAÇÃO ---

void atualizar_servo(uint16_t leitura_adc) {
    uint16_t largura_pulso_us = ((float)leitura_adc / VALOR_MAX_ADC) * (PULSO_MAXIMO_US - PULSO_MINIMO_US) + PULSO_MINIMO_US;
    pwm_set_gpio_level(PINO_SERVO, largura_pulso_us);
}


void atualizar_led(uint16_t leitura_adc) {
    uint16_t nivel_brilho;
    int distancia_do_centro = abs(leitura_adc - PONTO_CENTRAL_ADC);

    if (distancia_do_centro < ZONA_MORTA_JOYSTICK) {
        // Se estiver na zona morta, o LED fica completamente apagado
        nivel_brilho = 0;
    } else {
        // Se estiver fora, o brilho é proporcional à distância do centro
        nivel_brilho = ((float)distancia_do_centro / PONTO_CENTRAL_ADC) * WRAP_LED;
        // Garante que o valor não ultrapasse o máximo
        if (nivel_brilho > WRAP_LED) {
            nivel_brilho = WRAP_LED;
        }
    }
    
    pwm_set_gpio_level(PINO_LED, nivel_brilho);
}

// --- FUNÇÃO PRINCIPAL ---
int main() {
    // Inicializa a comunicação serial padrão para depuração
    stdio_init_all();

    // --- CONFIGURAÇÃO DO ADC ---
    adc_init();
    adc_gpio_init(PINO_JOYSTICK_Y);
    adc_select_input(0); // Canal 0 corresponde ao GPIO 26

    // --- CONFIGURAÇÃO DO SERVO ---
    gpio_set_function(PINO_SERVO, GPIO_FUNC_PWM);
    uint fatia_pwm_servo = pwm_gpio_to_slice_num(PINO_SERVO);
    pwm_set_clkdiv(fatia_pwm_servo, 125.0f);
    pwm_set_wrap(fatia_pwm_servo, 20000); // Configura o período do PWM para 20ms (50Hz)
    pwm_set_enabled(fatia_pwm_servo, true);
    // Define a posição inicial do servo como o centro
    uint16_t pulso_central = (PULSO_MINIMO_US + PULSO_MAXIMO_US) / 2;
    pwm_set_gpio_level(PINO_SERVO, pulso_central);

    // --- CONFIGURAÇÃO DO LED ---
    gpio_set_function(PINO_LED, GPIO_FUNC_PWM);
    uint fatia_pwm_led = pwm_gpio_to_slice_num(PINO_LED);
    pwm_set_clkdiv(fatia_pwm_led, 125.0f);
    pwm_set_wrap(fatia_pwm_led, WRAP_LED);
    pwm_set_enabled(fatia_pwm_led, true);
    // Garante que o LED comece totalmente apagado
    pwm_set_gpio_level(PINO_LED, 0);


    // --- LOOP PRINCIPAL ---
    while (true) {
        // Lê o valor analógico do joystick
        uint16_t leitura_adc_y = adc_read();
        
        // Atualiza a posição do servo e o brilho do LED com base na leitura
        atualizar_servo(leitura_adc_y);
        atualizar_led(leitura_adc_y);

        // Pausa de 20ms para sincronizar com o período do servo
        sleep_ms(20);
    }

    return 0; // Este ponto nunca é alcançado
}
