# Blind Dice

### A project dedicated to develop a dice for blind people.

## How will it be made?

#### It uses an MSP430F5529 to process and integrate an accelerometer (ADXL345), which will detect which face of the dice is facing up. After that, the microcontroller will make the buzzer beep according to the number on the face.

## How can I test the components?

```c
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

```
