#ifndef RTC_H
#define RTC_H

#include "pico/stdlib.h"
#include <stdint.h>

// Definições dos pinos e endereço do RTC DS1307
#define RTC_SDA      4
#define RTC_SCL      5
#define RTC_I2C      i2c0
#define DS1307_ADDR  0x68

// Estrutura para armazenar a hora lida do RTC
typedef struct {
    int horas;
    int minutos;
    int segundos;
    int dia;
} rtc_time_t;

/**
 * Inicializa a interface I2C e configura os pinos para comunicação com o RTC.
 */
void rtc_init_custom(void);

/**
 * Lê o RTC e preenche a estrutura rtc_time_t com o horário atual.
 */
void rtc_read_time(rtc_time_t *time_data);

/**
 * Converte um valor decimal para BCD.
 */
uint8_t dec_to_bcd(int val);

/**
 * Converte um valor BCD para decimal.
 */
int bcd_to_dec(uint8_t val);

#endif  // RTC_H
