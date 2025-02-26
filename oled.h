#ifndef OLED_H
#define OLED_H

#include <stdint.h>

// Tamanho do display OLED (128x64 pixels)
#define OLED_WIDTH   128
#define OLED_HEIGHT  64

// Define os pinos e a interface I2C utilizada pelo OLED.
#define OLED_SDA     14
#define OLED_SCL     15
#define OLED_I2C     i2c1

// Buffer global para o display (1 bit por pixel)
extern uint8_t oled_buffer[OLED_WIDTH * OLED_HEIGHT / 8];

/**
 * Inicializa o display OLED.
 * Configura o I2C, os pinos e inicializa a biblioteca do display.
 */
void oled_init_custom(void);

/**
 * Limpa o buffer do display (preenche com zeros).
 */
void oled_clear(void);

/**
 * Atualiza o display enviando o conteúdo do buffer para o hardware.
 */
void oled_update(void);

#endif // OLED_H
