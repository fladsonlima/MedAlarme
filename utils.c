#include "utils.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_GREEN_PIN 17
#define BUTTON_PIN 13

// Variável interna para o flag do medicamento tomado
static volatile bool medication_taken_flag_internal = false;

uint32_t wait_for_button_release(void) {
    uint32_t start = to_ms_since_boot(get_absolute_time());
    while (gpio_get(BUTTON_PIN) == 0) {
         sleep_ms(10);
    }
    return to_ms_since_boot(get_absolute_time()) - start;
}

void utils_blink_green(void) {
    gpio_put(LED_GREEN_PIN, 1);
    sleep_ms(300);
    gpio_put(LED_GREEN_PIN, 0);
}

void set_medication_taken_flag(bool flag) {
    medication_taken_flag_internal = flag;
}

bool get_medication_taken_flag(void) {
    return medication_taken_flag_internal;
}
