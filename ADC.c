#include <stdio.h>           // Entrada e saída padrão (printf, scanf)  
#include "pico/stdlib.h"     // Biblioteca padrão do Raspberry Pi Pico  
#include "hardware/pio.h"    // Controle de periféricos PIO  
#include "hardware/clocks.h" // Configuração dos clocks do microcontrolador  
#include "pico/bootrom.h"

#include "hardware/i2c.h"    // Comunicação I2C (sensores, displays)  
#include "lib/font.h"        // Fonte para exibição em displays  
#include "lib/ssd1306.h"     // Controle do display OLED SSD1306  

#include "hardware/adc.h"

#include "hardware/pwm.h"

//Protótipos das funções:
void init();
void gpio_irq_handler(uint gpio, uint32_t events);

// Definições de pinos e endereços I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x3C   // Endereço do dispositivo I2C

#define RED_LED 13    // Pino do LED vermelho
#define BLUE_LED 12     // Pino do LED azul
#define GREEN_LED 11    // Pino do LED verde
#define BUTTON_A 5      // Pino do botão A
#define BUTTON_B 6      // Pino do botão B

#define VRX_PIN 26
//Valor padrão 2028
#define VRY_PIN 27
//Valor padrão 1936
#define SW_PIN 22

// Variáveis globais:
static volatile uint32_t last_time = 0; // Armazena o tempo da última interrupção

bool pwm_enabled = true; 
uint slice_B;
uint slice_R;

uint8_t index_x;
uint8_t index_y;
ssd1306_t ssd; // Estrutura de controle do display OLED
bool cor = true;



void init(){
    stdio_init_all(); // Inicializa entrada e saída padrão
    
    // Configuraração do leds
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);


    // Configuraração do botão A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);


    // Configuraração do botão B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);


    // Configuraração do botão SW
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);
    gpio_set_irq_enabled_with_callback(SW_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_irq_handler);


    // Configurações do I2C
    i2c_init(I2C_PORT, 100 * 1000); // Inicializa I2C0 a 100kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // SDA
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // SCL
    gpio_pull_up(I2C_SDA); // Habilita pull-up para SDA
    gpio_pull_up(I2C_SCL); // Habilita pull-up para SCL
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, I2C_ADDR, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);


    // Configuraração do ADC
    adc_init();
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);


    // Configuraração do PWM
    gpio_set_function(BLUE_LED, GPIO_FUNC_PWM);
    slice_B = pwm_gpio_to_slice_num(BLUE_LED);
    pwm_set_wrap(slice_B, 4096);
    pwm_set_enabled(slice_B, pwm_enabled);
    

    gpio_set_function(RED_LED, GPIO_FUNC_PWM);
    slice_R = pwm_gpio_to_slice_num(RED_LED);
    pwm_set_wrap(slice_R, 4096);
    pwm_set_enabled(slice_R, pwm_enabled);
}


// Callback de interrupção para os botões
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    // Implementação de debounce (ignora eventos em menos de 300ms)
    if (current_time - last_time > 300000) {
        last_time = current_time;
        if (gpio == BUTTON_A) {
            pwm_enabled = !pwm_enabled ;
            if(!pwm_enabled){
                pwm_set_gpio_level(BLUE_LED, 0);
                pwm_set_gpio_level(RED_LED, 0);
            }
        }else if(gpio == SW_PIN){
            cor = !cor;
            gpio_put(GREEN_LED, !gpio_get(GREEN_LED)); // Alterna o LED verde
        }else {
            reset_usb_boot(0, 0);
        }
    }
}

// Função Principal
int main(){
    // Configuração dos leds, botões, adc, i2c, pwm
    init();
    
    while (true){
        adc_select_input(1); // joystick invertido, por isso é necessário usar 1 no lugar de 0
        uint16_t vrx_value = adc_read();
        adc_select_input(0); // joystick invertido, por isso é necessário usar 0 no lugar de 1
        uint16_t vry_value = adc_read();

        index_x = 36 + (((int)vrx_value - 2048) / 585 + 3) * 8;
        index_y = 4 + ((2048 - (int)vry_value) / 585 + 3) * 8;

        if(pwm_enabled){
            pwm_set_gpio_level(RED_LED, (abs((vrx_value - 2048)) < 150) ? 0 : abs(vrx_value - 2048) * 2);
            pwm_set_gpio_level(BLUE_LED, (abs((int)vry_value - 1940) < 150) ? 0 : abs(vry_value - 1940) * 2);
        }

        ssd1306_fill(&ssd, !cor);

        for(uint8_t i = 36; i < 92; i+=8){
            for(uint8_t j = 4; j < 60; j+=8){
                ssd1306_draw_char(&ssd, cor ? '-' : '_', i , j);
                if(i == index_x && j == index_y){
                    ssd1306_draw_char(&ssd, cor ? '#' : ' ', i, j);
                }
            }
        }
        
        ssd1306_send_data(&ssd); // Atualiza o display
    }
}

