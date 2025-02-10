/*
 * timer.h
 *
 *  Created on: 10 de fev de 2025
 *      Author: thiag
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <msp430.h>

typedef void (*callbackFunc)(void);

#define ACLK_FREQ 32768
#define MAX_COUNT 0xFFFF
enum timers {
    TIMER_A0,
    TIMER_A1,
    _COUNT_TIMERS
};

void init_timer(enum timers timer);
void set_timer_maxtime(enum timers timer, unsigned int time_ms);
void set_callback_timer(enum timers timer, callbackFunc timer_callback_new);

void set_timer_interruption(enum timers timer, int is_interruptable);
void reset_timer(enum timers timer);
void wait_timer(enum timers timer);
unsigned int get_timer(enum timers timer);

#endif /* TIMER_H_ */
