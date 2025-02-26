#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * Aguarda a liberação do botão e retorna o tempo de espera (em ms).
 */
uint32_t wait_for_button_release(void);

/**
 * Pisca o LED verde brevemente para indicar que o medicamento foi tomado.
 */
void utils_blink_green(void);

/**
 * Define o flag que indica que o medicamento foi tomado.
 */
void set_medication_taken_flag(bool flag);

/**
 * Retorna o valor atual do flag que indica que o medicamento foi tomado.
 */
bool get_medication_taken_flag(void);

#endif // UTILS_H