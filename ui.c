#include "ui.h"
#include "oled.h"
#include "display_utils.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "keypad.h"
#include "alarm.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>  // Para atoi

#define LED_BLUE_PIN    16
#define LED_YELLOW_PIN  18

// Variáveis de configuração (escopo local deste módulo)
static config_mode_t config_mode;
static int config_alarm_index = 0;
static int digit_count = 0;
static char input_buffer[3]; // Para armazenar dois dígitos + '\0'

void ui_init(void) {
    // Inicializações específicas de UI, se necessário.
}

void ui_update(const rtc_time_t *now, const char *status_msg) {
    // Primeiro, remove os alarmes passados, se houver
    alarm_remove_passed_alarms(now);

    // Buffers para cada linha
    char line1[32];
    char line2[32];
    char line3[64];
    char line4[64];

    // 1) Linha 1: "HORA 00 00 00"
    snprintf(line1, sizeof(line1), "HORA  %02d %02d %02d",
             now->horas, now->minutos, now->segundos);

    // 2) Linha 2: "PROX"
    strcpy(line2, "PROX");

    // 3) Linha 3: exibe todos os alarmes configurados
    if (all_alarms_taken()) {
            strcpy(line3, "ATE AMANHA");
        } else {
            // Caso contrário, monta a lista dos alarmes restantes em line3 (e, se necessário, em line4)
            line3[0] = '\0';
            line4[0] = '\0';
            for (int i = 0; i < MAX_ALARMS; i++) {
                if (alarmes[i].definido && !alarmes[i].disparou) {
                    char temp[12];
                    snprintf(temp, sizeof(temp), "%02d %02d ", alarmes[i].hora, alarmes[i].minuto);
                    if (strlen(line3) + strlen(temp) < 22) {
                        strcat(line3, temp);
                    } else if (strlen(line4) + strlen(temp) < 22) {
                        strcat(line4, temp);
                    } else {
                        strcat(line4, "+..");
                        break;
                    }
                }
            }
        }

    bool temAlarme = false;
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarmes[i].definido && !alarmes[i].disparou) {
            temAlarme = true;
            break;
        }
    }

    // 4) Linha 4: status dos alarmes ou alerta
    if (strstr(status_msg, "ALERTA") != NULL) {
        strcpy(line4, "HORA DO REMEDIO");
    } else if (all_alarms_taken()) {
        strcpy(line4, "");
    } else if (temAlarme) {
        strcpy(line4, "ALARMES ATIVOS");
    } else {
        // Se não há nenhum alarme definido ou todos foram removidos
        strcpy(line4, "SEM ALARME");
    }

    // Limpa o buffer do display e desenha cada linha em posições fixas
    oled_clear();
    ssd1306_draw_string(oled_buffer, 0,   0, line1);  // Y=0
    ssd1306_draw_string(oled_buffer, 0,  16, line2);  // Y=16
    ssd1306_draw_string(oled_buffer, 0,  32, line3);  // Y=32
    ssd1306_draw_string(oled_buffer, 0,  48, line4);  // Y=48
    oled_update();

    // Pisca o LED azul a cada 500ms
    uint32_t current_ms = to_ms_since_boot(get_absolute_time());
    if ((current_ms / 500) % 2 == 0)
        gpio_put(LED_BLUE_PIN, 1);
    else
        gpio_put(LED_BLUE_PIN, 0);

    // Se houver alerta, pisca o LED amarelo a cada 250ms
    if (strstr(status_msg, "ALERTA") != NULL) {
        if ((current_ms / 250) % 2 == 0)
            gpio_put(LED_YELLOW_PIN, 1);
        else
            gpio_put(LED_YELLOW_PIN, 0);
    } else {
        gpio_put(LED_YELLOW_PIN, 0);
    }
}

void ui_edit_mode(void) {
    // Inicializa as variáveis de configuração para o modo de edição
    config_mode = CONFIG_HOUR;
    config_alarm_index = 0;
    digit_count = 0;
    input_buffer[0] = '\0';

    oled_clear();
    {
        char prompt[48];
        sprintf(prompt, "Configuracao iniciada\nALARME %d: Hora:", config_alarm_index + 1);
        draw_multiline_text(oled_buffer, 0, 0, prompt, 14);
        oled_update();
    }

    printf("Entrando no modo de configuração.\n");
    printf("Digite dois dígitos para a hora (00) e dois para os minutos (00).\n");
    printf("Pressione '#' para confirmar ou '*' para finalizar.\n");

    bool config_finished = false;
    while (!config_finished) {
        char key = keypad_get_key();
        if (key != 0) {
            if (key == '*' && digit_count == 0) {
                // Finaliza a configuração se '*' for pressionado sem inserir nada
                printf("Configuração finalizada pelo usuário. %d alarmes definidos.\n", config_alarm_index);
                oled_clear();
                draw_multiline_text(oled_buffer, 0, 0, "Configuracao Finalizada", 14);
                oled_update();
                sleep_ms(1000);
                break;
            }

            switch (config_mode) {
                case CONFIG_HOUR:
                    if (key >= '0' && key <= '9') {
                        input_buffer[digit_count++] = key;
                        input_buffer[digit_count] = '\0';
                        char disp[32];
                        sprintf(disp, "ALARME %d Hora %s", config_alarm_index + 1, input_buffer);
                        oled_clear();
                        draw_multiline_text(oled_buffer, 0, 0, disp, 14);
                        oled_update();

                        if (digit_count == 2) {
                            int hour = atoi(input_buffer);
                            if (hour >= 0 && hour < 24) {
                                alarmes[config_alarm_index].hora = hour;
                                digit_count = 0;
                                input_buffer[0] = '\0';
                                config_mode = CONFIG_MINUTE;
                                oled_clear();
                                draw_multiline_text(oled_buffer, 0, 0, "ALARME Min", 14);
                                oled_update();
                            } else {
                                digit_count = 0;
                                input_buffer[0] = '\0';
                                printf("Hora inválida. Tente novamente.\n");
                            }
                        }
                    }
                    break;

                case CONFIG_MINUTE:
                    if (key >= '0' && key <= '9') {
                        input_buffer[digit_count++] = key;
                        input_buffer[digit_count] = '\0';
                        char disp[32];
                        sprintf(disp, "ALARME %d: Min: %s", config_alarm_index + 1, input_buffer);
                        oled_clear();
                        draw_multiline_text(oled_buffer, 0, 0, disp, 14);
                        oled_update();

                        if (digit_count == 2) {
                            int minute = atoi(input_buffer);
                            if (minute >= 0 && minute < 60) {
                                alarmes[config_alarm_index].minuto = minute;
                                digit_count = 0;
                                input_buffer[0] = '\0';
                                config_mode = CONFIG_WAIT_CONFIRM;

                                oled_clear();
                                {
                                    char conf[40];
                                    sprintf(conf, "ALARME %d-- %02d-%02d", config_alarm_index + 1,
                                            alarmes[config_alarm_index].hora, alarmes[config_alarm_index].minuto);

                                    // Exibição da configuração do alarme
                                    draw_multiline_text(oled_buffer, 0, 0, conf, 14);
                                    draw_multiline_text(oled_buffer, 0, 16, "PRESS CERQUILHA", 14);
                                    draw_multiline_text(oled_buffer, 0, 32, "P CONFIRMAR", 14);
                                    draw_multiline_text(oled_buffer, 0, 48, "OU ASTERISCO P FINALIZAR", 14);
                                }
                                oled_update();

                            } else {
                                digit_count = 0;
                                input_buffer[0] = '\0';
                                printf("Minuto inválido. Tente novamente.\n");
                            }
                        }
                    }
                    break;

                case CONFIG_WAIT_CONFIRM:
                    if (key == '#') {
                        alarmes[config_alarm_index].definido = true;
                        printf("Alarme %d configurado: %02d:%02d\n", config_alarm_index + 1,
                               alarmes[config_alarm_index].hora, alarmes[config_alarm_index].minuto);
                        config_alarm_index++;

                        if (config_alarm_index >= MAX_ALARMS) {
                            oled_clear();
                            draw_multiline_text(oled_buffer, 0, 0, "Configuracao Concluida", 14);
                            oled_update();
                            sleep_ms(1000);
                            config_finished = true;
                            break;
                        } else {
                            config_mode = CONFIG_HOUR;
                            digit_count = 0;
                            input_buffer[0] = '\0';
                            oled_clear();
                            char prompt[32];
                            sprintf(prompt, "ALARME %d: Hora:", config_alarm_index + 1);
                            draw_multiline_text(oled_buffer, 0, 0, prompt, 14);
                            oled_update();
                        }
                    } else if (key == '*') {
                        printf("Configuração finalizada pelo usuário. %d alarme(s) definido(s).\n", config_alarm_index);
                        oled_clear();
                        draw_multiline_text(oled_buffer, 0, 0, "Configuracao Finalizada", 14);
                        oled_update();
                        sleep_ms(1000);
                        config_finished = true;
                        break;
                    }
                    break;
            }
        }
        sleep_ms(50);
    }
}