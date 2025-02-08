/*
 * gpio.h
 *
 *  Created on: Jan 9, 2025
 *      Author: nikomania
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <msp430.h>
#include <stdint.h>

#define RED_LED BIT0
#define GREEN_LED BIT7

void init_gpio();

void red_led_on();
void red_led_off();
void red_led_toggle();

void green_led_on();
void green_led_off();
void green_led_toggle();

void buzzer_on();
void buzzer_off();
void buzzer_toggle();

uint8_t is_button_pressed();
uint8_t is_button_clicked();

#endif /* GPIO_H_ */
