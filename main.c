#include <msp430.h>
#include <i2c.h>
#include <gpio.h>
#include <adxl345.h>
#include <timer.h>
#include <stdint.h>

#define TIME_TO_CHECK_DICE_MS 5000

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    init_gpio();
    init_i2c();
    init_ADXL345();
    init_timerA0();

    set_timerA0_maxtime(TIME_TO_CHECK_DICE_MS);
    set_callback_timerA0(buzzer_toggle);

    __enable_interrupt();

    struct accel accel_data;

    // test
    volatile int x, y, z;
    volatile int clicked = 0;
    volatile int activate_buzzer_alarm = 0;
    while(1) {
        clicked = is_button_clicked();
        ADXL345_get_data(&accel_data);
        x = accel_data.x;
        y = accel_data.y;
        z = accel_data.z;

        if (clicked) {
            activate_buzzer_alarm = !activate_buzzer_alarm;
            reset_timerA0();
            set_timerA0_interruption(activate_buzzer_alarm);
        }

        green_led_toggle();
        __delay_cycles(100000);
    }
}
