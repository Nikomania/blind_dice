/*
 * timer.h
 *
 *  Created on: Nov 28, 2024
 *      Author: nikomania
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <msp430.h>

void config_timerA0(void);

unsigned int get_timerA0();
void set_timerA0_maxtime(unsigned int time);
void reset_timerA0();
void wait_timerA0();
void reset_wait_timerA0();


void config_timerA1(void);

void set_timerA1_maxtime(unsigned int time);
void reset_timerA1();
void wait_timerA1();
int times_up_A1();
void reset_wait_timerA1();

#endif /* TIMER_H_ */
