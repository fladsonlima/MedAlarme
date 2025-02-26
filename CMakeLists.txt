cmake_minimum_required(VERSION 3.13)

# Importa o SDK do Pico
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(MyProject VERSION 1.0)

# Inicializa o SDK do Pico
pico_sdk_init()

# Adiciona os arquivos fonte do projeto
add_executable(my_project
    src/main.c
    src/keypad.c
    src/rtc.c
    src/alarm.c
    src/buzzer.c
    src/oled.c
    src/ui.c
    src/utils.c
)

# Inclui o diretório raiz (ou outro, se necessário) para os headers
target_include_directories(my_project PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Habilita a saída USB para stdio (opcional) e desabilita o UART, se preferir
pico_enable_stdio_usb(my_project 1)
pico_enable_stdio_uart(my_project 0)

# Gera arquivos extras, como UF2, .hex, etc.
pico_add_extra_outputs(my_project)

# Liga as bibliotecas necessárias do Pico SDK
target_link_libraries(my_project pico_stdlib hardware_i2c hardware_pwm)
