/*
 * Timer.c
 *
 *  Created on: Nov 28, 2024
 *      Author: nikomania
 */

#include <timer.h>

void init_timerA0(void) {
    TA0CTL = TASSEL__ACLK
    | MC__UP
    | ID_3
    | TACLR;
}

void set_timerA0_interruption(int is_interruptable) {
    if (is_interruptable) {
        TA0CCTL0 |= CCIE;
        return;
    }
    TA0CCTL0 &= ~CCIE;
}

void set_timerA0_maxtime(unsigned int time_ms) {
    // explanation in notebook
    const unsigned int time = time_ms / 125;
    TA0CCR0 = time << 9;
}

void reset_timerA0() {
    TA0CTL |= TACLR;
    TA0CTL &= ~TAIFG;
}

void wait_timerA0() {
    while((TA0CTL&TAIFG) == 0);
}

void reset_wait_timerA0() {
    reset_timerA0();
    wait_timerA0();
}

unsigned int get_timerA0() {
    return TA0R;
}
