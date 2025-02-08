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

void initI2C_Master(void);
uint8_t i2cTest(uint8_t slave_addr);
uint8_t i2cTestNack();
void i2cSend(
        uint8_t slave_addr,
        uint8_t register_addr,
        uint8_t data
);
void i2cRead(
        uint8_t slave_addr,
        uint8_t register_addr,
        uint8_t* byte
);
void i2cReadVect(
        uint8_t slave_addr,
        uint8_t register_addr,
        uint8_t byte_count,
        volatile uint8_t* data
);


#endif /* I2C_H_ */
