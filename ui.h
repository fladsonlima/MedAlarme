#ifndef UI_H
#define UI_H

#include "rtc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>  // Para atoi, se necessário

#ifdef __cplusplus
extern "C" {
#endif

// Define os estados para a configuração via teclado
typedef enum {
    CONFIG_HOUR,
    CONFIG_MINUTE,
    CONFIG_WAIT_CONFIRM
} config_mode_t;

/**
 * Inicializa o módulo de UI.
 */
void ui_init(void);

/**
 * Atualiza o display OLED com informações atuais (horário, status) e gerencia os LEDs.
 *
 * @param now O horário atual.
 * @param status_msg Mensagem de status a ser exibida.
 */
void ui_update(const rtc_time_t *now, const char *status_msg);

/**
 * Entra no modo de edição dos alarmes via teclado.
 */
void ui_edit_mode(void);

#ifdef __cplusplus
}
#endif

#endif // UI_H
