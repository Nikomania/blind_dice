#include <msp430.h>
#include <i2c.h>
#include <gpio.h>
#include <adxl345.h>
#include <stdint.h>


int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    init_gpio();
    initI2C_Master();
    ADXL345Config();

    struct accel accel_data;
    volatile int clicked = 0;

    // test
    volatile int x, y, z;

    while(1) {
        clicked = is_button_clicked();
        ADXL345GetData(&accel_data);
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
