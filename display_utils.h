#ifndef DISPLAY_UTILS_H
#define DISPLAY_UTILS_H

#include <stdint.h>

/**
 * @brief Desenha texto em múltiplas linhas, quebrando a cada `max_chars` caracteres.
 *
 * @param buffer      Buffer do display (ex.: ssd1306_buffer).
 * @param x           Posição X em pixels (0 = canto esquerdo).
 * @param y           Posição Y inicial em pixels (0 = topo).
 * @param text        String a ser desenhada.
 * @param max_chars   Máximo de caracteres por linha (ex.: 20).
 */
void draw_multiline_text(uint8_t *buffer, int x, int y, const char *text, int max_chars);

/**
 * @brief Desenha texto com quebra automática por largura de pixels (simples).
 *
 * @param buffer     Buffer do display (ex.: ssd1306_buffer).
 * @param x          Posição X inicial em pixels (0 = canto esquerdo).
 * @param y          Posição Y inicial em pixels (0 = topo).
 * @param text       String a ser desenhada.
 * @param max_width  Largura máxima em pixels (ex.: 120).
 */
void draw_text_wrap(uint8_t *buffer, int x, int y, const char *text, int max_width);

#endif
