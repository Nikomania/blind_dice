/*
 * Timer.c
 *
 *  Created on: Nov 28, 2024
 *      Author: nikomania
 */

#include <timer.h>

// Função para configurar o timer TA0
void config_timerA0(void) {
    TA0CTL = TASSEL__ACLK
    | MC__UP
    | TACLR;
}

void config_timerA1() {
    TA1CTL = TASSEL__ACLK
    | MC__UP
    | TACLR
    | ID_2;
}

void set_timerA0_maxtime(unsigned int time) {
    TA0CCR0 = time;
}

void set_timerA1_maxtime(unsigned int time) {
    TA1CCR0 = time;
}

void reset_timerA0() {
    TA0CTL |= TACLR;
    TA0CTL &= ~TAIFG;
}

void wait_timerA0() {
    while((TA0CTL&TAIFG) == 0) {} // Esperar a flag
}

void reset_wait_timerA0() {
    reset_timerA0();
    wait_timerA0();
}

void reset_timerA1() {
    TA1CTL |= TACLR;
    TA1CTL &= ~TAIFG;
}

void wait_timerA1() {
    while((TA1CTL&TAIFG) == 0) {} // Esperar a flag
}

int times_up_A1() {
    return (TA1CTL&TAIFG) == 0;
}

void reset_wait_timerA1() {
    reset_timerA1();
    wait_timerA1();
}

unsigned int get_timerA0() {
    return TA0R;
}

