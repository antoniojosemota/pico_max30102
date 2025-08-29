#ifndef MAX30102_H_
#define MAX30102_H_

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// O endereço I2C é o mesmo
#define MAX30102_I2C_ADDR 0x57

// Mapa de Registradores do MAX30102
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C // RED
#define REG_LED2_PA 0x0D // IR
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

bool max30102_init(i2c_inst_t *i2c);
bool max30102_read_fifo(uint32_t *ir_data, uint32_t *red_data);
void max30102_reset();

#endif /* MAX30102_H_ */