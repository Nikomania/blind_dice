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
    struct accel accel_data;
    int avgs_axis[_COUNT_AXIS];
    unsigned int batch, i;
    enum axis axis_index;
    for (axis_index =  X_AXIS; axis_index < _COUNT_AXIS; axis_index++) {
        for (batch = 0; batch < (1 << batches_calibration_exp); batch++) {
            int avg_acc = 0;
            for (i = 0; i < (1 << max_samples_avg_exp); i++) {
                ADXL345_get_data(&accel_data);
                switch (axis_index) {
                    case X_AXIS:
                        avg_acc += accel_data.x;
                        break;
                    case Y_AXIS:
                        avg_acc += accel_data.y;
                        break;
                    case Z_AXIS:
                        avg_acc += accel_data.z;
                        break;
                }
            }
            int avg_temp = avg_acc >> max_samples_avg_exp;

            if (batch) {
                avgs_axis[axis_index] = (avg_temp + avgs_axis[axis_index]) >> 1;
            } else {
                avgs_axis[axis_index] = avg_temp;
            }
        }
    }

    if (module(max(avgs_axis, _COUNT_AXIS, 1)) > VAL_ACCEL_NO_OFFSET) {
        const enum axis max_axis_index = (enum axis)max_index(avgs_axis, _COUNT_AXIS, 1);
        const int axis_x_signal = (avgs_axis[X_AXIS] < 0) ? -1 : 1;
        const int axis_y_signal = (avgs_axis[Y_AXIS] < 0) ? -1 : 1;
        const int axis_z_signal = (avgs_axis[Z_AXIS] < 0) ? -1 : 1;
        offset = (struct accel) {
            (max_axis_index != X_AXIS) ? - avgs_axis[X_AXIS] : axis_x_signal * (GRAVITY_ACCEL - module(avgs_axis[X_AXIS])),
            (max_axis_index != Y_AXIS) ? - avgs_axis[Y_AXIS] : axis_y_signal * (GRAVITY_ACCEL - module(avgs_axis[Y_AXIS])),
            (max_axis_index != Z_AXIS) ? - avgs_axis[Z_AXIS] : axis_z_signal * (GRAVITY_ACCEL - module(avgs_axis[Z_AXIS]))
        };
        return;
    }

    red_led_on();
    while(1);
}
