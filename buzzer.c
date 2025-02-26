#include "buzzer.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

void buzzer_init(void) {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice_num, &config, true);
    pwm_set_wrap(slice_num, 1250);  // Aproximadamente 25 kHz
    pwm_set_gpio_level(BUZZER_PIN, 625);  // 50% duty cycle
    pwm_set_enabled(slice_num, false);
}

void buzzer_set(bool on) {
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    pwm_set_enabled(slice_num, on);
}