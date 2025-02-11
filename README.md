# Controle de Servomotor por PWM no Raspberry Pi Pico W

## Sobre o Projeto

Este projeto demonstra o controle de um **servomotor** (micro servo padrão) por meio de **PWM** no **Raspberry Pi Pico W**, utilizando o [Pico SDK](https://github.com/raspberrypi/pico-sdk) e o simulador online [Wokwi](https://wokwi.com/). O objetivo é gerar um sinal de aproximadamente **50Hz** de frequência (período de 20ms) e, em seguida, ajustar o ciclo de trabalho (duty cycle) em diferentes larguras de pulso para controlar o ângulo do servomotor de maneira suave.

As principais funcionalidades incluídas são:

1. **Configurar** a GPIO 22 para fornecer PWM de 50Hz (ciclo de 20ms).  
2. **Posicionar** o servo em 0°, 90° e 180°, permanecendo por 5 segundos em cada posição.  
3. **Realizar** uma varredura contínua entre 0° e 180° (ida e volta) com incrementos de 5µs e atraso de 10ms, garantindo um movimento suave.  
4. **Analisar** o comportamento de um LED conectado à GPIO 12 (via [BitDogLab](https://bitdoglab.com/)) durante o acionamento do servomotor.  

---

## Diagrama Esquemático e Fluxograma no Wokwi

O esquema de montagem, simulado na plataforma **Wokwi**, é ilustrado a seguir. Nele, temos:

- **1x Raspberry Pi Pico W**  
- **1x Servomotor (micro servo padrão)**  
- **1x LED RGB (ou LED simples)** opcional na GPIO 12 para análise de comportamento  

<p align="center">
  <img src="esquema.gif" alt="Esquema de Simulação no Wokwi" width="701">
</p>

**Legenda:**
- GPIO 22 -> Sinal PWM do servo  
- 5V ou 3.3V -> Alimentação do servo (depende do modelo no Wokwi)  
- GND -> Terra comum para Pico e servo  
- GPIO 12 -> LED para observação (opcional)

---

## 🛠 Instalação e Uso

1. **Clone este repositório** em seu ambiente de desenvolvimento ou Raspberry Pi Pico W:
   ```sh
   https://github.com/engleonardorodrigues/pwm-with-RP2040
   ```
2. **Compile** e **carregue** o código no Raspberry Pi Pico W utilizando o **Pico SDK e o VSCode**.  
3. **Execute** o código: após o upload, o firmware iniciará automaticamente no Pico W.
4. **Simulação no Wokwi**:  
   - Clicar em **diagram.json** e ver a simulação acontecendo.

---

## Descrição do Código

O código em C faz uso das bibliotecas de **hardware PWM** do RP2040. Abaixo, os principais pontos:

1. **Configuração do PWM**:  
   - Define-se a **GPIO 22** como saída PWM (`gpio_set_function(PIN, GPIO_FUNC_PWM)`).  
   - Ajusta-se a frequência para **~50Hz** (20ms de período) configurando o *wrap* do PWM em **19.999** e o **divisor de clock** em **125** (125 MHz / 125 = 1 MHz → 1µs por “tick”).  

2. **Posicionamento inicial**:  
   - O servo é posicionado em 0°, 90° ou 180° por meio do ajuste do pulso (ex.: 500µs para 0°, 1470µs para 90°, 2400µs para 180°).  
   - A cada posição, aguarda-se **5 segundos**.

3. **Varredura suave**:  
   - Um loop “sobe” de 500µs para 2400µs em incrementos de 5µs, pausando **10ms** a cada incremento.  
   - Em seguida, o loop “desce” no sentido inverso (2400µs para 500µs).  
   - Isso permite observar um movimento gradativo e **fluido** da haste do servomotor.

4. **Observação do LED (GPIO 12)**:  
   - Opcionalmente, um LED é configurado na GPIO 12 para analisar comportamento luminoso no [BitDogLab](https://bitdoglab.com/).  
   - Durante a varredura do servo, podem ocorrer **pequenas variações** no LED (cintilação, mudança de brilho) devido à carga elétrica do servo ou pela forma como o PWM e a fonte de alimentação interagem na simulação.

---

## Conclusão

O projeto ilustra o uso do **PWM** no **Raspberry Pi Pico W** para controle de servomotores, abordando desde a **configuração de frequência** até o **controle de ângulo** em diferentes posições e em movimentos suaves. 

---

## 📩 Contato

Caso tenha dúvidas ou sugestões, entre em contato:

- **Desenvolvedor**: Leonardo Rodrigues
- **Linkedin**: https://www.linkedin.com/in/leonardorodrigues7/