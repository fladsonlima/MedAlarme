#include "keypad.h"
#include "pico/stdlib.h"

const uint KEYPAD_ROWS[KEYPAD_NUM_ROWS] = {2, 3, 6, 7};
const uint KEYPAD_COLS[KEYPAD_NUM_COLS] = {8, 9, 10};

char keypad_keys[KEYPAD_NUM_ROWS][KEYPAD_NUM_COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void keypad_init(void) {
    // Inicializa as linhas do teclado como entrada com pull-up
    for (int i = 0; i < KEYPAD_NUM_ROWS; i++) {
        gpio_init(KEYPAD_ROWS[i]);
        gpio_set_dir(KEYPAD_ROWS[i], GPIO_IN);
        gpio_pull_up(KEYPAD_ROWS[i]);
    }
    // Inicializa as colunas como saída em nível alto
    for (int i = 0; i < KEYPAD_NUM_COLS; i++) {
        gpio_init(KEYPAD_COLS[i]);
        gpio_set_dir(KEYPAD_COLS[i], GPIO_OUT);
        gpio_put(KEYPAD_COLS[i], 1);
    }
}

char keypad_get_key(void) {
    // Varre cada coluna para detectar se alguma tecla foi pressionada.
    for (int col = 0; col < KEYPAD_NUM_COLS; col++) {
        gpio_put(KEYPAD_COLS[col], 0);
        sleep_ms(1);
        for (int row = 0; row < KEYPAD_NUM_ROWS; row++) {
            if (gpio_get(KEYPAD_ROWS[row]) == 0) {
                sleep_ms(20); // debounce
                if (gpio_get(KEYPAD_ROWS[row]) == 0) {
                    // Aguarda a liberação da tecla para evitar repetições
                    while (gpio_get(KEYPAD_ROWS[row]) == 0) { 
                        sleep_ms(10); 
                    }
                    gpio_put(KEYPAD_COLS[col], 1);
                    return keypad_keys[row][col];
                }
            }
        }
        gpio_put(KEYPAD_COLS[col], 1);
    }
    return 0; // Nenhuma tecla foi pressionada
}