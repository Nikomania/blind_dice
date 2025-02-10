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

void init_i2c(void);
uint8_t i2c_test(uint8_t slave_addr);
void i2c_send(
        uint8_t slave_addr,
        uint8_t register_addr,
        uint8_t data
);
void i2c_read(
        uint8_t slave_addr,
        uint8_t register_addr,
        uint8_t* byte
);
void i2c_read_vect(
        uint8_t slave_addr,
        uint8_t register_addr,
        uint8_t byte_count,
        volatile uint8_t* data
);


#endif /* I2C_H_ */
