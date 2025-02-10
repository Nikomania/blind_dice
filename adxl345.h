/*
 * adxl345.h
 *
 *  Created on: 8 de fev de 2025
 *      Author: thiag
 */

#ifndef ADXL345_H_
#define ADXL345_H_

#include <msp430.h>
#include <stdint.h>

#define ADXL345_ADDR 0x53
#define ADXL345_DATA_ADDR 0x32
#define ADXL345_BYTES_SENT 6
#define ADXL345_DEVID_ADDR 0x00
#define ADXL345_DEVID_VAL 0xE5
#define ADXL345_POWER_CTL 0x2D

#define POS_VAL_ACCEL_NO_OFFSET 128
#define NEG_VAL_ACCEL_NO_OFFSET -128
#define GRAVITY_ACCEL 256

struct accel {
    int x;
    int y;
    int z;
};

enum axis {
    X_AXIS,
    Y_AXIS,
    Z_AXIS,
    _COUNT_AXIS
};

void init_ADXL345();
void ADXL345_calibrate(uint8_t max_samples_avg, uint8_t batches_calibration);
void ADXL345_check_devid();
void ADXL345_check_i2c();
void ADXL345_get_data(struct accel* accel_data);


#endif /* ADXL345_H_ */
