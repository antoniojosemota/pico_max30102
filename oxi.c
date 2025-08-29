#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "max30100/max30100.h" // Alterado
#include "algorithm/algorithm.h"

// Configuração dos pinos I2C
#define I2C_PORT i2c1
#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

// Buffers para os dados brutos do sensor (agora uint32_t)
uint32_t ir_buffer[BUFFER_SIZE];
uint32_t red_buffer[BUFFER_SIZE];

int main() {
    stdio_init_all();
    sleep_ms(200);

    printf("Iniciando monitor de SpO2 e frequência cardíaca...\n");

    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    printf("I2C inicializado.\n");

    printf("Inicializando o sensor MAX30102...\n"); // Alterado

    if (!max30102_init(I2C_PORT)) { // Alterado
        printf("Falha ao inicializar o sensor MAX30102.\n"); // Alterado
        while (1);
    } else {
        printf("Sensor MAX30102 inicializado com sucesso.\n"); // Alterado
    }

    int32_t spo2;
    int8_t spo2_valid;
    int32_t heart_rate;
    int8_t hr_valid;
    uint32_t buffer_idx = 0;

    while (1) {
        while (buffer_idx < BUFFER_SIZE) {
            uint32_t ir, red; // Alterado para uint32_t
            max30102_read_fifo(&ir, &red); // Alterado
            ir_buffer[buffer_idx] = ir;
            red_buffer[buffer_idx] = red;
            buffer_idx++;
            sleep_ms(10);
        }
        
        calc_spo2_and_hr(&spo2, &spo2_valid, &heart_rate, &hr_valid, ir_buffer, red_buffer);

        if (hr_valid && spo2_valid) {
            printf("HR: %ld bpm, SpO2: %ld %%\n", heart_rate, spo2);
        } else {
            // Se os valores de IR forem muito baixos, significa que não há dedo no sensor
            if(ir_buffer[BUFFER_SIZE-1] < 50000) {
                 printf("Por favor, coloque o dedo no sensor.\n");
            } else {
                 printf("Calculando... Por favor, mantenha o dedo firme.\n");
            }
        }
        
        for (int i = 0; i < (BUFFER_SIZE - FS); i++) {
            ir_buffer[i] = ir_buffer[i + FS];
            red_buffer[i] = red_buffer[i + FS];
        }
        buffer_idx = BUFFER_SIZE - FS;
    }
    return 0;
}