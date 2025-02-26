#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>
#include "rtc.h"

#define MAX_ALARMS 3

typedef struct {
    int hora;
    int minuto;
    bool disparou;  // Alarme começou a tocar
    bool definido;  // Alarme configurado
    bool tomado;    // Remédio confirmado pelo usuário
} alarm_time_t;

#ifdef __cplusplus
extern "C" {
#endif

extern alarm_time_t alarmes[MAX_ALARMS];
extern int last_alarm_day;

/**
 * Inicializa os alarmes, definindo-os como não configurados.
 */
void alarm_init(void);

/**
 * Se um novo dia ocorreu, reinicia os flags de disparo dos alarmes.
 */
void alarm_reset_daily(const rtc_time_t *now);

/**
 * Verifica os alarmes configurados com base no horário atual.
 *
 * @param now O horário atual.
 * @param alarm_active Ponteiro para a variável (volatile) que indica se o alarme está ativo.
 * @param status Buffer para atualizar a mensagem de status.
 */
void alarm_check(const rtc_time_t *now, volatile bool *alarm_active, char *status);

int alarm_remove_passed_alarms(const rtc_time_t *now);

bool all_alarms_taken(void);

#ifdef __cplusplus
}
#endif

#endif // ALARM_H