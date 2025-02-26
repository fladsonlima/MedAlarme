#include "alarm.h"
#include <string.h>
#include <stdio.h>

alarm_time_t alarmes[MAX_ALARMS] = {
    {-1, -1, false, false},
    {-1, -1, false, false},
    {-1, -1, false, false}
};

int last_alarm_day = -1;
int current_alarm_index = -1;

void alarm_init(void) {
    for (int i = 0; i < MAX_ALARMS; i++) {
        alarmes[i].hora = -1;
        alarmes[i].minuto = -1;
        alarmes[i].disparou = false;
        alarmes[i].definido = false;
        alarmes[i].tomado = false;
    }
    last_alarm_day = -1;
}

void alarm_reset_daily(const rtc_time_t *now) {
    if (now->dia != last_alarm_day) {
        for (int i = 0; i < MAX_ALARMS; i++) {
            alarmes[i].disparou = false;
            alarmes[i].tomado = false;  // Se quiser resetar para novo dia
        }
        last_alarm_day = now->dia;
        printf("Novo dia! Alarmes reativados.\n");
        printf("Pressione * se quiser configurar os alarmes.\n");
    }
}

void alarm_check(const rtc_time_t *now, volatile bool *alarm_active, char *status) {
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarmes[i].definido &&
            !alarmes[i].disparou &&
            now->horas == alarmes[i].hora &&
            now->minutos == alarmes[i].minuto &&
            now->segundos == 0) {
                *alarm_active = true;
                alarmes[i].disparou = true;
                current_alarm_index = i; // Salva qual alarme está ativo
                strcpy(status, "ALERTA! Tome o rem.");
                printf("Alarme %d ativado! Pressione o botao para desligar.\n", i);
        }
    }
}

static bool alarm_is_past(const rtc_time_t *now, const alarm_time_t *alarm) {
    // Considera que o alarme já passou se a hora for menor ou se a hora for igual e o minuto for menor
    if (alarm->hora < now->horas) {
        return true;
    } else if (alarm->hora == now->horas && alarm->minuto < now->minutos) {
        return true;
    }
    return false;
}

// Remove todos os alarmes que já passaram deslocando os demais
int alarm_remove_passed_alarms(const rtc_time_t *now) {
    int removedCount = 0;
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarmes[i].definido) {
            // Só remove se o horário já passou e o remédio foi confirmado (tomado)
            if (alarm_is_past(now, &alarmes[i]) && alarmes[i].tomado) {
                // Desloca os alarmes para a esquerda
                for (int j = i; j < MAX_ALARMS - 1; j++) {
                    alarmes[j] = alarmes[j + 1];
                }
                // Limpa a última posição
                alarmes[MAX_ALARMS - 1].hora = -1;
                alarmes[MAX_ALARMS - 1].minuto = -1;
                alarmes[MAX_ALARMS - 1].disparou = false;
                alarmes[MAX_ALARMS - 1].definido = false;
                alarmes[MAX_ALARMS - 1].tomado = false;
                
                removedCount++;
                // Volta uma posição para reavaliar o novo alarme que caiu na posição i
                i--;
            }
        }
    }
    return removedCount;
}


// Função auxiliar que verifica se todos os alarmes definidos já foram tomados.
bool all_alarms_taken(void) {
    bool foundOne = false;
    for (int i = 0; i < MAX_ALARMS; i++) {
        if (alarmes[i].definido) {
            foundOne = true;
            // Se este alarme ainda não foi confirmado pelo usuário (tomado), retorna false
            if (!alarmes[i].tomado) {
                return false;
            }
        }
    }
    // Se não encontrou nenhum alarme definido, retorna false (para não exibir "ATE AMANHA" se não há alarmes)
    // Se encontrou e todos estão "tomados", retorna true
    return foundOne;
}
