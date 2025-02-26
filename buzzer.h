#ifndef BUZZER_H
#define BUZZER_H

#include <stdbool.h>
#include "pico/stdlib.h"

// Define o pino conectado ao buzzer.
// Se necessário, ajuste conforme seu hardware.
#define BUZZER_PIN 12

/**
 * Inicializa o buzzer, configurando o pino BUZZER_PIN para PWM.
 * O PWM é configurado para aproximadamente 25 kHz com 50%% de duty cycle.
 */
void buzzer_init(void);

/**
 * Ativa ou desativa o buzzer.
 *
 * @param on Se true, ativa o buzzer; se false, desativa.
 */
void buzzer_set(bool on);

#endif // BUZZER_H