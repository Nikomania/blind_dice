/*
 * Timer.c
 *
 *  Created on: Nov 28, 2024
 *      Author: nikomania
 */

#include <timer.h>

callbackFunc timer_callback = 0;

void init_timerA0(void) {
    TA0CTL = TASSEL__ACLK
    | MC__UP
    | ID_3
    | TACLR;
}

void set_timerA0_interruption(int is_interruptable) {
    if (is_interruptable) {
        TA0CTL |= TAIE;
        return;
    }
    TA0CTL &= ~TAIE;
}

void set_callback_timerA0(callbackFunc timer_callback_new) {
    timer_callback = timer_callback_new;
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

unsigned int get_timerA0_ms() {
    const unsigned int time = TA0R;
    return ((time >> 7) * 125) >> 2;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void isr_ta0(void){
    int n;
    n = __even_in_range(TA0IV,0xE);
    switch(n) {
        case 0x0: break; //Nenhuma interrpção pendente
        case 0x2: break; //TA0CCTL1.CCIFG
        case 0x4: break; //TA0CCTL2.CCIFG
        case 0x6: break; //TA0CCTL3.CCIFG
        case 0x8: break; //TA0CCTL4.CCIFG
        case 0xA: break; //TA0CCTL5.CCIFG (Não existe no TA0)
        case 0xC: break; //TA0CCTL6.CCIFG (Não existe no TA0)
        case 0xE: //TA0CTL.TAIV
            if (timer_callback != 0) {
                timer_callback();
            }
        break;
    }
}

