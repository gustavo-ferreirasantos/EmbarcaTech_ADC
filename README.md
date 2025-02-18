# EmbarcaTech_ADC

## 📌 DESCRIÇÃO
Este projeto explora o uso do conversor analógico-digital (ADC) no microcontrolador RP2040 (Raspberry Pi Pico W) e a integração com a placa educacional BitDogLab. A implementação utiliza um joystick para controlar a intensidade de dois LEDs RGB via PWM e mover um quadrado no display SSD1306. Além disso, o botão do joystick e o botão A adicionam funcionalidades extras, como alternância de LEDs e modificação da borda do display.

---

## 🎯 OBJETIVOS
✅ Compreender o funcionamento do conversor analógico-digital (ADC) no RP2040.  
✅ Controlar a intensidade dos LEDs RGB com base nos valores do joystick.  
✅ Exibir a posição do joystick no display SSD1306 com um quadrado móvel de 8x8 pixels.  
✅ Aplicar o protocolo I2C para comunicação com o display.  
✅ Implementar interrupções (IRQ) para lidar com eventos dos botões.  
✅ Implementar debouncing via software para evitar acionamentos falsos dos botões.  

---

## 🛠️ PRÉ-REQUISITOS

- 🛠️ HARDWARE NECESSÁRIO:
  
  - Placa de desenvolvimento BitDogLab (RP2040).  
  - Joystick conectado às GPIOs 26 (X) e 27 (Y).  
  - LED RGB conectado às GPIOs 11, 12 e 13.  
  - Botão do Joystick conectado à GPIO 22.   
  - Botão A conectado à GPIO 5.  
  - Display SSD1306 conectado via I2C (GPIO 14 e GPIO 15).  

  
- 🖥 SOFTWARE NECESSÁRIO:
  - Raspberry Pi Pico SDK configurado.  
  - CMake para compilação.  
  - VS Code com a extensão Raspberry Pi Pico. 

---

## 🚀 COMO EXECUTAR

1️⃣ **Clone este repositório:**

       git clone https://github.com/gustavo-ferreirasantos/EmbarcaTech_ADC

2️⃣ Abra o projeto no VS Code e importe.

3️⃣ Compile e carregue o código na BitDogLab usando o SDK do Raspberry Pi Pico.

---

## 🎥 Vídeo de demonstração
🔗 <u>[Assista aqui](https://youtu.be/MBy4VnQCs48)</u>
