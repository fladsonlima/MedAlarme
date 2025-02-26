#include "rtc.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"

uint8_t dec_to_bcd(int val) {
    return (uint8_t)(((val / 10) << 4) | (val % 10));
}

int bcd_to_dec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

void rtc_init_custom(void) {
    // Inicializa a interface I2C com 100 kHz
    i2c_init(RTC_I2C, 100000);
    // Configura os pinos para função I2C
    gpio_set_function(RTC_SDA, GPIO_FUNC_I2C);
    gpio_set_function(RTC_SCL, GPIO_FUNC_I2C);
    // Habilita os pull-ups
    gpio_pull_up(RTC_SDA);
    gpio_pull_up(RTC_SCL);
}

void rtc_read_time(rtc_time_t *time_data) {
    uint8_t reg = 0x00;
    uint8_t buf[7];
    // Envia o endereço do registrador para leitura
    i2c_write_blocking(RTC_I2C, DS1307_ADDR, &reg, 1, true);
    // Lê 7 bytes a partir do registrador 0
    i2c_read_blocking(RTC_I2C, DS1307_ADDR, buf, 7, false);
    // Converte os valores lidos (BCD) para decimal
    time_data->segundos = bcd_to_dec(buf[0] & 0x7F);
    time_data->minutos  = bcd_to_dec(buf[1]);
    time_data->horas    = bcd_to_dec(buf[2] & 0x3F);
    time_data->dia      = bcd_to_dec(buf[4]);
}
