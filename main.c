#include <msp430.h>
#include <i2c.h>
#include <gpio.h>
#include <adxl345.h>
#include <timer.h>
#include <stdint.h>


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    init_gpio();
    init_i2c();
    init_ADXL345();

    struct accel accel_data;
    volatile int clicked = 0;

    // test
    volatile int x, y, z;

    __enable_interrupt();

    while(1) {
        clicked = is_button_clicked();
        ADXL345_get_data(&accel_data);
        x = accel_data.x;
        y = accel_data.y;
        z = accel_data.z;

        if (clicked) {
            buzzer_toggle();
        }

        green_led_toggle();
        __delay_cycles(100000);
    }
}
