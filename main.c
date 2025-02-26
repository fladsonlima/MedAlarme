/*
UNIT07 - ATIVIDADE: TAREFA DE ENVIO - PROJETO FINAL
Aluno: Fládson Lima
EmbarcaTech - Mentoria 3
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "keypad.h"
#include "rtc.h"
#include "alarm.h"
#include "oled.h"
#include "buzzer.h"
#include "ui.h"
#include "utils.h"

#define LED_BLUE_PIN    16  // LED Azul: Sistema configurado
#define LED_GREEN_PIN   17  // LED Verde: Medicamento tomado
#define LED_YELLOW_PIN  18  // LED Amarelo: Alarme ativo
#define BUTTON_PIN      13  // Botão

// Variável global para indicar se o alarme está ativo.
volatile bool alarm_active = false;

typedef enum { NORMAL_MODE, EDIT_MODE } app_mode_t;
app_mode_t app_mode = NORMAL_MODE;

// Buffer de status para o display
char status[32] = "Bom dia, vovo! Sem alarmes.";

extern int current_alarm_index;

void button_callback(uint gpio, uint32_t events) {
    if (alarm_active) {
        if (current_alarm_index >= 0 && current_alarm_index < MAX_ALARMS) {
            alarmes[current_alarm_index].tomado = true;
            printf("Medicamento do alarme %d tomado!\n", current_alarm_index);
        }

        alarm_active = false;
        strcpy(status, "Medicamento tomado!");
        set_medication_taken_flag(true);
        printf("Alarme desligado.\n");

        current_alarm_index = -1;
    }
}

int main(void) {
    stdio_init_all();

    // Inicializa os LEDs
    gpio_init(LED_BLUE_PIN);
    gpio_set_dir(LED_BLUE_PIN, GPIO_OUT);
    
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_GREEN_PIN, 0);
    
    gpio_init(LED_YELLOW_PIN);
    gpio_set_dir(LED_YELLOW_PIN, GPIO_OUT);
    gpio_put(LED_YELLOW_PIN, 0);
    
    // Inicializa o botão (GP13)
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    
    // Inicializa os módulos
    keypad_init();
    rtc_init_custom();
    oled_init_custom();
    buzzer_init();
    alarm_init();
    ui_init();
    
    rtc_time_t now;
    while (true) {
        rtc_read_time(&now);
        
        // Verifica se é um novo dia e reinicia o flag de disparo dos alarmes
        alarm_reset_daily(&now);
        
        // Verifica os alarmes; a função alarm_check atualiza alarm_active e status
        alarm_check(&now, &alarm_active, status);
        
        // Ativa ou desativa o buzzer conforme o estado do alarme
        buzzer_set(alarm_active);
        
        // Atualiza o display e o blink dos LEDs (azul e amarelo) via UI
        ui_update(&now, status);
        
        // Se o usuário pressionar '*' no keypad, entra no modo de edição
        char key = keypad_get_key();
        if (key == '*') {
            app_mode = EDIT_MODE;
            ui_edit_mode();
            app_mode = NORMAL_MODE;
        }
        
        // Se o flag de "medicamento tomado" estiver ativo, pisca o LED verde
        if (get_medication_taken_flag()) {
            utils_blink_green();
            set_medication_taken_flag(false);
        }
        
        sleep_ms(100);
    }
    return 0;
}
