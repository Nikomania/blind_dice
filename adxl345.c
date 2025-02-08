/*
 * adxl345.c
 *
 *  Created on: 8 de fev de 2025
 *      Author: thiag
 */

#include <adxl345.h>
#include <i2c.h>
#include <gpio.h>

void ADXL345CheckI2C() {
    if (i2cTest(ADXL345_ADDR) == 0) {
        red_led_on();
        while(1);
    }
}

void ADXL345CheckDevId() {
    uint8_t devid;
    i2cRead(ADXL345_ADDR, ADXL345_DEVID_ADDR, &devid);
    if (devid != ADXL345_DEVID_VAL) {
        while(1) {
            red_led_toggle();
            __delay_cycles(100000);
        }
    }
}

void ADXL345Config() {
    ADXL345CheckI2C();
    ADXL345CheckDevId();

    i2cSend(ADXL345_ADDR, ADXL345_POWER_CTL, 0); // wakeup
    i2cSend(ADXL345_ADDR, ADXL345_POWER_CTL, 16); // auto_sleep
    i2cSend(ADXL345_ADDR, ADXL345_POWER_CTL, 8); // measure
}

void ADXL345GetData(struct accel* accel_data) {
    uint8_t data[ADXL345_BYTES_SENT];

    i2cReadVect(ADXL345_ADDR, ADXL345_DATA_ADDR, ADXL345_BYTES_SENT, data);

    accel_data->x = data[0] | (data[1] << 8);
    accel_data->y = data[2] | (data[3] << 8);
    accel_data->z = data[4] | (data[5] << 8);
}
