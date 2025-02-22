/*
 * gpio.c
 *
 *  Created on: Jan 9, 2025
 *      Author: nikomania
 */
#include <gpio.h>

void init_gpio() {
    // Configura os pinos GPIO para I2C
    // UCB0: P3.0 = SDA, P3.1 = SCL
    P3SEL |= BIT0 | BIT1;                   // Atribui os pinos ao módulo I2C UCB0
    P3REN |= BIT0 | BIT1;                   // Habilita os resistores de pull-up/down
    P3OUT |= BIT0 | BIT1;                   // Configura como pull-up

    // Leds for feedback
    P1DIR |= RED_LED; //LedRed = P1.0 = saída (Turn on in case of error)
    P1OUT &= ~RED_LED; //LedRed apagado

    P4DIR |= GREEN_LED; //LedRed = P1.0 = saída (Turn on in case of success)
    P4OUT &= ~GREEN_LED; //LedRed apagado

    // buzzer
    P4DIR |= BIT3;
    P4OUT &= ~BIT3;

    // button
    P1DIR &= ~BIT2; // input
    P1SEL &= ~BIT2; // gpio
    P1REN |= BIT2; // resistor
    P1OUT |= BIT2; // pull-up
}

void red_led_on() {
    P1OUT |= RED_LED;
}

void red_led_off() {
    P1OUT &= ~RED_LED;
}

void red_led_toggle() {
    P1OUT ^= RED_LED;
}

void green_led_on() {
    P4OUT |= GREEN_LED;
}

void green_led_off() {
    P4OUT &= ~GREEN_LED;
}

void green_led_toggle() {
    P4OUT ^= GREEN_LED;
}

void buzzer_on() {
    P4OUT |= BIT3;
}

void buzzer_off() {
    P4OUT &= ~BIT3;
}

void buzzer_toggle() {
    P4OUT ^= BIT3;
}

uint8_t is_button_pressed() {
    return (P1IN & BIT2) == 0;
}

uint8_t is_button_clicked() {
    static volatile int was_pressed = 0;
    volatile int pressed = is_button_pressed();

    if (pressed && !was_pressed) {
        was_pressed = 1;
        return 1;
        __delay_cycles(100000); // debounce
    } else if (!pressed) {
        was_pressed = 0;
    }
    return 0;
}
