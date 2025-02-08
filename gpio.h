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

void buzzer_on();
void buzzer_off();
void buzzer_toggle();

uint8_t is_button_pressed();

#endif /* GPIO_H_ */
