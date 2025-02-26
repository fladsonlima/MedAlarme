MedAlarme – Gerenciador de Alarmes para Medicamentos

O MedAlarme é um sistema embarcado desenvolvido para ajudar na administração correta dos medicamentos. Criado e testado 100% no simulador Wokwi, o projeto utiliza o Raspberry Pi Pico W e é programado em C (pico-sdk). Ele integra um display OLED para exibir informações, um RTC para manter o tempo preciso, além de LEDs, um buzzer, um keypad e um botão para interação com o usuário.

Principais Arquivos do Projeto
main.c:
Integra os módulos do sistema, inicializa os periféricos e gerencia o loop principal que atualiza o tempo, verifica os alarmes e processa as entradas do usuário.

alarm.c / alarm.h:
Contêm a lógica para configuração, verificação, disparo e reset dos alarmes diários. Gerenciam o estado de cada alarme (definido, disparado e, opcionalmente, confirmado/tomado).

buzzer.c / buzzer.h:
Controlam o buzzer para emitir alertas sonoros quando um alarme é ativado.

oled.c / oled.h:
Responsáveis pela inicialização e atualização do display OLED (via driver SSD1306), que exibe a hora, alarmes e mensagens de status.

ssd1306_font.h, ssd1306_i2c.c, ssd1306_i2c.h:
Implementam os drivers e a fonte para o display OLED, garantindo a renderização correta dos dados no display.

display_utils.c / display_utils.h:
Fornecem funções auxiliares para formatar e desenhar textos e gráficos no buffer do display.

keypad.c / keypad.h:
Gerenciam a leitura do teclado matricial, permitindo a configuração dos alarmes pelo usuário.

rtc.c / rtc.h:
Lidam com a comunicação com o módulo RTC (DS1307) para leitura e manutenção do tempo.

ui.c / ui.h:
Implementam a interface do usuário, exibindo informações no display e controlando os modos de configuração.

utils.c / utils.h:
Contêm funções utilitárias que auxiliam em operações diversas, como formatação de dados e controle de fluxo.

diagram.json:
Arquivo de definição do circuito no simulador Wokwi, detalhando os componentes e suas interligações.

CMakeLists.txt:
Arquivo de configuração do projeto para compilação com o pico-sdk.

wokwi-project.txt:
Informações complementares do projeto disponibilizadas no ambiente Wokwi.

Execução do Projeto
O projeto foi desenvolvido e testado integralmente no simulador Wokwi. Para executá-lo:

No Wokwi:
Basta abrir o projeto através do link ou importar o arquivo ZIP, onde a simulação mostra a interação entre os componentes (RTC, OLED, LEDs, buzzer, keypad e botão).

Localmente (em hardware real):

Configure o ambiente de desenvolvimento com o pico-sdk e o CMake.
Compile o projeto no VS Code.
Carregue o firmware gerado (arquivo UF2) no Raspberry Pi Pico W via USB.

Referências:
Projeto Wokwi – Display OLED com Pico W em C: https://wokwi.com/projects/421456571126424577
Raspberry Pi Pico Examples – SSD1306 I2C: https://github.com/raspberrypi/pico-examples/tree/master/i2c/ssd1306_i2c
Projeto Wokwi Alternativo – Correção de Exibição no OLED: https://wokwi.com/projects/393154286020492289
BitDogLab – Display OLED: https://github.com/BitDogLab/BitDogLab-C/tree/main/display_oled
Documentação Wokwi – Membrane Keypad: https://docs.wokwi.com/pt-BR/parts/wokwi-membrane-keypad
Documentação Wokwi – Board SSD1306: https://docs.wokwi.com/pt-BR/parts/board-ssd1306
Diagramas do Projeto – Miro: https://miro.com/app/board/uXjVIaVYfIs=/
Proteus 8 Free Trial – Diagrama Elétrico: https://www.labcenter.com/free-trial/

Considerações Finais

O MedAlarme demonstra como é possível desenvolver um sistema embarcado eficiente e de baixo custo para a administração de medicamentos, utilizando o Raspberry Pi Pico W e diversas interfaces de hardware. Com uma abordagem modular e uma interface intuitiva, o projeto garante a correta configuração e disparo dos alarmes, contribuindo para a adesão ao tratamento medicamentoso.
