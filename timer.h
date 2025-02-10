/*
 * timer.h
 *
 *  Created on: Nov 28, 2024
 *      Author: nikomania
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <msp430.h>

#define ACLK_FREQ 32768
#define MAX_COUNT 0xFFFF

void init_timerA0(void);

unsigned int get_timerA0();
void set_timerA0_maxtime(unsigned int time_ms);
void reset_timerA0();
void wait_timerA0();
void reset_wait_timerA0();
void set_timerA0_interruption(int is_interruptable);

#endif /* TIMER_H_ */
