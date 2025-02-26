#ifndef KEYPAD_H
#define KEYPAD_H

#include "pico/stdlib.h"

#define KEYPAD_NUM_ROWS 4
#define KEYPAD_NUM_COLS 3

// Pinos para as linhas e colunas do keypad
extern const uint KEYPAD_ROWS[KEYPAD_NUM_ROWS];
extern const uint KEYPAD_COLS[KEYPAD_NUM_COLS];

// Mapeamento dos caracteres do keypad (layout 4x3)
extern char keypad_keys[KEYPAD_NUM_ROWS][KEYPAD_NUM_COLS];

/**
 * Inicializa os pinos do keypad.
 * As linhas são configuradas como entrada com pull-up,
 * e as colunas como saída, inicialmente em nível alto.
 */
void keypad_init(void);

/**
 * Varre o keypad e retorna o caractere pressionado.
 * Retorna 0 se nenhuma tecla for detectada.
 */
char keypad_get_key(void);

#endif  // KEYPAD_H