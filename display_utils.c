#include "display_utils.h"
#include "ssd1306_font.h"
#include <string.h>      // para strncpy, strlen
#include "ssd1306.h"     // para ssd1306_draw_string(), ssd1306_draw_char()

// Tamanho de caractere e altura de linha (ajuste se necessário)
#define CHAR_WIDTH   6  // 5 px + 1 px de espaçamento
#define LINE_HEIGHT  10 // Aproximadamente 8 px de altura da fonte + 2 px de espaçamento

void draw_multiline_text(uint8_t *buffer, int x, int y, const char *text, int max_chars) {
    int len = strlen(text);
    int offset = 0;
    char line[128];  // Buffer temporário para a linha

    while (offset < len) {
        int chunk_len = (len - offset > max_chars) ? max_chars : (len - offset);
        int last_space = -1;  // Posição do último espaço encontrado

        // Evita quebrar palavras no meio, exceto quando necessário
        for (int i = 0; i < chunk_len; i++) {
            if (text[offset + i] == ' ') {
                last_space = i;
            }
        }

        if (last_space != -1 && last_space < chunk_len - 1) {
            chunk_len = last_space + 1;
        }

        // Copia o trecho
        strncpy(line, &text[offset], chunk_len);
        line[chunk_len] = '\0';

        // Ajuste para exibir '*' e '#' corretamente
        for (int i = 0; i < chunk_len; i++) {
            if (line[i] == '#' || line[i] == '*') {
                ssd1306_draw_char(buffer, x + (i * 6), y, line[i]); // Exibe corretamente caracteres especiais
            }
        }

        // Exibe a string completa
        ssd1306_draw_string(buffer, x, y, line);

        // Avança para a próxima linha
        y += LINE_HEIGHT + 3;
        offset += chunk_len;
    }
}

void draw_text_wrap(uint8_t *buffer, int x, int y, const char *text, int max_width) {
    int cursor_x = x;
    int cursor_y = y;

    for (int i = 0; text[i] != '\0'; i++) {
        // Se for quebra de linha manual (\n), pula de linha
        if (text[i] == '\n') {
            cursor_x = x;
            cursor_y += LINE_HEIGHT;
            continue;
        }

        // Se ultrapassar a largura máxima, pula de linha
        if (cursor_x + CHAR_WIDTH > max_width) {
            cursor_x = x;
            cursor_y += LINE_HEIGHT;
        }

        // Desenha caractere convertendo para índice correto da fonte
        ssd1306_draw_char(buffer, cursor_x, cursor_y, get_font_index(text[i]));
        // Avança cursor
        cursor_x += CHAR_WIDTH;
    }
}