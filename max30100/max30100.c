#include "max30100.h"
#include <stdio.h>

static i2c_inst_t *i2c_port;

static void max30102_write_reg(uint8_t reg, uint8_t value) {
    uint8_t buf[2] = {reg, value};
    i2c_write_blocking(i2c_port, MAX30102_I2C_ADDR, buf, 2, false);
}

static void max30102_read_reg(uint8_t reg, uint8_t *buf, uint8_t len) {
    i2c_write_blocking(i2c_port, MAX30102_I2C_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_port, MAX30102_I2C_ADDR, buf, len, false);
}

void max30102_reset() {
    max30102_write_reg(REG_MODE_CONFIG, 0x40);
}

bool max30102_init(i2c_inst_t *i2c) {
    i2c_port = i2c;

    uint8_t part_id = 0;
    max30102_read_reg(REG_PART_ID, &part_id, 1);
    printf("Lendo Part ID do sensor... Valor: 0x%02X\n", part_id);

    if (part_id != 0x15) {
        printf("Part ID incorreto. Esperado 0x15 para MAX30102.\n");
        return false;
    }

    max30102_reset();
    sleep_ms(100);

    // Configurações do Sensor
    max30102_write_reg(REG_INTR_ENABLE_1, 0xc0); // Habilita interrupção de FIFO quase cheio e novos dados
    max30102_write_reg(REG_INTR_ENABLE_2, 0x00);
    max30102_write_reg(REG_FIFO_WR_PTR, 0x00);
    max30102_write_reg(REG_OVF_COUNTER, 0x00);
    max30102_write_reg(REG_FIFO_RD_PTR, 0x00);
    max30102_write_reg(REG_FIFO_CONFIG, 0x4f); // Média de 4 amostras, rolar FIFO, FIFO quase cheio em 17
    max30102_write_reg(REG_MODE_CONFIG, 0x03);   // Modo SpO2
    max30102_write_reg(REG_SPO2_CONFIG, 0x27); // ADC 4096nA, 100 amostras/seg, pulso de 411us (18 bits)
    max30102_write_reg(REG_LED1_PA, 0x24);       // Corrente LED Vermelho ~7mA
    max30102_write_reg(REG_LED2_PA, 0x24);       // Corrente LED IR ~7mA

    return true;
}

bool max30102_read_fifo(uint32_t *ir_data, uint32_t *red_data) {
    uint8_t buffer[6];
    max30102_read_reg(REG_FIFO_DATA, buffer, 6);

    // Reconstrói os dados de 18 bits a partir dos 3 bytes para cada LED
    *red_data = ((uint32_t)buffer[0] << 16) | ((uint32_t)buffer[1] << 8) | buffer[2];
    *ir_data  = ((uint32_t)buffer[3] << 16) | ((uint32_t)buffer[4] << 8) | buffer[5];
    
    // Mascara para manter apenas os 18 bits válidos
    *red_data &= 0x03FFFF;
    *ir_data  &= 0x03FFFF;

    return true;
}