#include "oled.h"
#include "ssd1306.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <string.h>

// Define o buffer global para o display
uint8_t oled_buffer[OLED_WIDTH * OLED_HEIGHT / 8];

void oled_init_custom(void) {
    // Inicializa o I2C para o OLED com 100 kHz
    i2c_init(OLED_I2C, 100000);
    gpio_set_function(OLED_SDA, GPIO_FUNC_I2C);
    gpio_set_function(OLED_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(OLED_SDA);
    gpio_pull_up(OLED_SCL);
    
    // Inicializa a biblioteca do display (SSD1306)
    ssd1306_init();
    // Limpa o buffer e envia para o display
    memset(oled_buffer, 0, sizeof(oled_buffer));
    ssd1306_send_buffer(oled_buffer, sizeof(oled_buffer));
}

void oled_clear(void) {
    memset(oled_buffer, 0, sizeof(oled_buffer));
}

void oled_update(void) {
    ssd1306_send_buffer(oled_buffer, sizeof(oled_buffer));
}
