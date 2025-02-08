/*
 * i2c.h
 *
 *  Created on: Jan 9, 2025
 *      Author: nikomania
 */

#ifndef I2C_H_
#define I2C_H_

#include <msp430.h>
#include <stdint.h>

#define ADXL345_ADDR 0x53
#define ADXL345_DATA_ADDR 0x32
#define ADXL345_BYTES_SENT 6
#define ADXL345_DEVID_ADDR 0x00
#define ADXL345_DEVID_VAL 0xE5
#define ADXL345_POWER_CTL 0x2D

// Protótipos das funções
void initI2C_Master(void);
uint8_t i2cTest(uint8_t slave_addr);
uint8_t i2cSend(uint8_t slave_addr, uint8_t register_addr, uint8_t data);
uint8_t i2cRead(uint8_t slave_addr, uint8_t register_addr, uint8_t* byte);
uint8_t i2cADXL345Config();
uint8_t i2cReadVect(uint8_t slave_addr, uint8_t register_addr, uint8_t byte_count, volatile uint8_t* data);
uint8_t i2cTestNack();

extern volatile uint8_t RXData;                // Variável para armazenar dados recebidos


#endif /* I2C_H_ */
