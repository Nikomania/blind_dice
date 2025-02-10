/*
 * adxl345.c
 *
 *  Created on: 8 de fev de 2025
 *      Author: thiag
 */

#include <adxl345.h>
#include <i2c.h>
#include <gpio.h>
#include <utils.h>

struct accel offset = {0, 0, 0};

void ADXL345_check_i2c() {
    if (i2c_test(ADXL345_ADDR) == 0) {
        red_led_on();
        while(1);
    }
}

void ADXL345_check_devid() {
    uint8_t devid;
    i2c_read(ADXL345_ADDR, ADXL345_DEVID_ADDR, &devid);
    if (devid != ADXL345_DEVID_VAL) {
        while(1) {
            red_led_toggle();
            __delay_cycles(100000);
        }
    }
}

void init_ADXL345() {
    ADXL345_check_i2c();
    ADXL345_check_devid();

    i2c_send(ADXL345_ADDR, ADXL345_POWER_CTL, 0); // wakeup
    i2c_send(ADXL345_ADDR, ADXL345_POWER_CTL, 16); // auto_sleep
    i2c_send(ADXL345_ADDR, ADXL345_POWER_CTL, 8); // measure
}

void ADXL345_get_data(struct accel* accel_data) {
    uint8_t data[ADXL345_BYTES_SENT];

    i2c_read_vect(ADXL345_ADDR, ADXL345_DATA_ADDR, ADXL345_BYTES_SENT, data);

    accel_data->x = (data[0] | (data[1] << 8)) + offset.x;
    accel_data->y = (data[2] | (data[3] << 8)) + offset.y;
    accel_data->z = (data[4] | (data[5] << 8)) + offset.z;
}

void ADXL345_calibrate(uint8_t max_samples_avg_exp, uint8_t batches_calibration_exp) {
    int avg_x = 0, avg_y = 0, avg_z = 0;

    int avg_x_temp = 0, avg_y_temp = 0, avg_z_temp = 0;
    int avg_x_acc = 0, avg_y_acc = 0, avg_z_acc = 0;
    struct accel accel_data;

    unsigned int batch, i;
    for (batch = 0; batch < (1 << batches_calibration_exp); batch++) {
        for (i = 0; i < (1 << max_samples_avg_exp); i++) {
            ADXL345_get_data(&accel_data);
            avg_x_acc += accel_data.x;
            avg_y_acc += accel_data.y;
            avg_z_acc += accel_data.z;
        }
        avg_x_temp = avg_x_acc >> max_samples_avg_exp;
        avg_y_temp = avg_y_acc >> max_samples_avg_exp;
        avg_z_temp = avg_z_acc >> max_samples_avg_exp;
        avg_x_acc = 0;
        avg_y_acc = 0;
        avg_z_acc = 0;

        if (batch) {
            avg_x = (avg_x_temp + avg_x) / 2;
            avg_y = (avg_y_temp + avg_y) / 2;
            avg_z = (avg_z_temp + avg_z) / 2;
        } else {
            avg_x = avg_x_temp;
            avg_y = avg_y_temp;
            avg_z = avg_z_temp;
        }
    }

    int avgs[3] = {avg_x, avg_y, avg_z};
    // tem q levar em consideração o sentido dos outros eixos, n so o sentido do eixo com maior valor absoluto
    if (max(avgs, 3) > POS_VAL_ACCEL_NO_OFFSET) {
        const unsigned int max_axis_index = max_index(avgs, 3);
        offset = (struct accel){(max_axis_index != 0) ? - avg_x : GRAVITY_ACCEL - avg_x, (max_axis_index != 1) ? - avg_y : GRAVITY_ACCEL - avg_y, (max_axis_index != 2) ? - avg_z : GRAVITY_ACCEL - avg_z};
        return;
    }

    if (min(avgs, 3) < NEG_VAL_ACCEL_NO_OFFSET) {
        const unsigned int min_axis_index = min_index(avgs, 3);
        offset = (struct accel){(min_axis_index != 0) ? avg_x : -(GRAVITY_ACCEL + avg_x), (min_axis_index != 1) ? avg_y : -(GRAVITY_ACCEL + avg_y), (min_axis_index != 2) ? avg_z : -(GRAVITY_ACCEL + avg_z)};
        return;
    }

    red_led_on();
    while(1);
}
