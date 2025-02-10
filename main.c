#include <msp430.h>
#include <i2c.h>
#include <gpio.h>
#include <adxl345.h>
#include <timer.h>
#include <utils.h>
#include <stdint.h>

#define TIME_TO_THROW_DICE_MS 5000
#define TIME_TO_CHECK_DICE_MS 1000
#define MAX_SAMPLES_AVG_EXP 3 // 2 ^ 3 = 8
#define BATCHES_CALIBRATION_EXP 5 // 2 ^ 5 = 32

#define MAX_AVG_NO_ACCEL 13 // 0.5 / 9.8 * 256 (accelerometer sees g's, not m/s², so 9.8 m/s² = 1 g = 256 in register)
#define MIN_AVG_FACE_UP 242 // 9.3 / 9.8 * 256
#define MAX_AVG_FACE_UP 269 // 10.3 / 9.8 * 256

enum faces {
    FACE_1,
    FACE_2,
    FACE_3,
    FACE_4,
    FACE_5,
    FACE_6,
    MOVING
};

volatile enum faces face_up = MOVING;

volatile int avg_x = 0, avg_y = 0, avg_z = 0;

void dice_has_fallen();
void check_dice_balance();
void set_face_up(unsigned int axis_index, char is_positive);
void play_face_sound();

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    init_gpio();
    init_i2c();
    init_ADXL345();

    init_timer(TIMER_A0);
    set_timer_maxtime(TIMER_A0, TIME_TO_THROW_DICE_MS);
    set_callback_timer(TIMER_A0, dice_has_fallen);

    init_timer(TIMER_A1);
    set_timer_maxtime(TIMER_A1, TIME_TO_CHECK_DICE_MS);
    set_callback_timer(TIMER_A1, check_dice_balance);

    __enable_interrupt();

    struct accel accel_data;

    int avg_x_acc = 0, avg_y_acc = 0, avg_z_acc = 0;
    unsigned int sample_size = 0;

    while (!is_button_clicked());
    ADXL345_calibrate(MAX_SAMPLES_AVG_EXP, BATCHES_CALIBRATION_EXP);

    char clicked = 0;
    while(1) {
        clicked = is_button_clicked();
        ADXL345_get_data(&accel_data);
        avg_x_acc += accel_data.x;
        avg_y_acc += accel_data.y;
        avg_z_acc += accel_data.z;
        sample_size++;

        if (sample_size >= (1 << MAX_SAMPLES_AVG_EXP)) {
            avg_x = avg_x_acc >> MAX_SAMPLES_AVG_EXP;
            avg_y = avg_y_acc >> MAX_SAMPLES_AVG_EXP;
            avg_z = avg_z_acc >> MAX_SAMPLES_AVG_EXP;

            avg_x_acc = 0;
            avg_y_acc = 0;
            avg_z_acc = 0;
            sample_size = 0;
        }

        if (clicked) {
            reset_timer(TIMER_A0);
            set_timer_interruption(TIMER_A0, 1);
        }

        if (face_up != MOVING) {
            play_face_sound();
        }

        green_led_toggle();
        __delay_cycles(100000);
    }
}

void dice_has_fallen() {
    set_timer_interruption(TIMER_A0, 0);
    set_timer_interruption(TIMER_A1, 1);
}

void check_dice_balance() {
    int avgs[_COUNT_AXIS] = {avg_x, avg_y, avg_z};

    unsigned int no_accel_axis = 0, face_up_axis = 0;
    enum axis axis_index, face_up_index = _COUNT_AXIS;
    for (axis_index = X_AXIS; axis_index < _COUNT_AXIS; axis_index++) {
        if (module(avgs[axis_index]) < MAX_AVG_NO_ACCEL) {
            no_accel_axis++;
        } else if (
            module(avgs[axis_index]) > MIN_AVG_FACE_UP &&
            module(avgs[axis_index]) < MAX_AVG_FACE_UP
        ) {
            face_up_axis++;
            face_up_index = axis_index;
        }
    }

    if (no_accel_axis != 2 || face_up_axis != 1) {
        return;
    }

    set_timer_interruption(TIMER_A1, 0);
    set_face_up(face_up_index, avgs[face_up_index] > 0);
}

void set_face_up(enum axis face_up_axis, char is_positive) {
    switch (face_up_axis) {
        case X_AXIS:
            face_up = (is_positive) ? FACE_1 : FACE_6;
        break;
        case Y_AXIS:
            face_up = (is_positive) ? FACE_4 : FACE_3;
        break;
        case Z_AXIS:
            face_up = (is_positive) ? FACE_2: FACE_5;
        break;
    }
}

void play_face_sound() {
    // gambiarra to test
    int i;
    for (i = 0; i < (face_up + 1) * 2; i++) {
        buzzer_toggle();
        __delay_cycles(1000000);
    }
}
