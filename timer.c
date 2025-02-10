/*
 * timer.c
 *
 *  Created on: 10 de fev de 2025
 *      Author: thiag
 */

#include <timer.h>

callbackFunc timer_callback[_COUNT_TIMERS];

void init_timer(enum timers timer) {
    switch (timer) {
        case TIMER_A0:
            TA0CTL = TASSEL__ACLK
                | MC__UP
                | ID_3
                | TACLR;
            break;
        case TIMER_A1:
            TA1CTL = TASSEL__ACLK
                | MC__UP
                | ID_3
                | TACLR;
            break;
    }
}

void set_timer_interruption(enum timers timer, int is_interruptable) {
    if (is_interruptable) {
        switch (timer) {
            case TIMER_A0:
                TA0CTL |= TAIE;
                break;
            case TIMER_A1:
                TA1CTL |= TAIE;
                break;
        }
        return;
    }
    switch (timer) {
        case TIMER_A0:
            TA0CTL &= ~TAIE;
            break;
        case TIMER_A1:
            TA1CTL &= ~TAIE;
            break;
    }
}

void set_callback_timer(enum timers timer, callbackFunc timer_callback_new) {
    timer_callback[timer] = timer_callback_new;
}

void set_timer_maxtime(enum timers timer, unsigned int time_ms) {
    // explanation in notebook
    unsigned int time = time_ms / 125;
    time = time << 9;
    switch (timer) {
        case TIMER_A0:
            TA0CCR0 = time;
            break;
        case TIMER_A1:
            TA1CCR0 = time;
            break;
    }
}

void reset_timer(enum timers timer) {
    switch (timer) {
        case TIMER_A0:
            TA0CTL |= TACLR;
            TA0CTL &= ~TAIFG;
            break;
        case TIMER_A1:
            TA1CTL |= TACLR;
            TA1CTL &= ~TAIFG;
            break;
    }
}

void wait_timer(enum timers timer) {
    switch (timer) {
        case TIMER_A0:
            while((TA0CTL&TAIFG) == 0);
            break;
        case TIMER_A1:
            while((TA1CTL&TAIFG) == 0);
            break;
    }
}

unsigned int get_timer_ms(enum timers timer) {
    unsigned int time;
    switch (timer) {
        case TIMER_A0:
            time = TA0R;
            break;
        case TIMER_A1:
            time = TA1R;
            break;
    }
    return ((time >> 7) * 125) >> 2;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void isr_ta0(void){
    int n;
    n = __even_in_range(TA0IV,0xE);
    switch(n) {
        case 0x0: break; //Nenhuma interrupção pendente
        case 0x2: break; //TA0CCTL1.CCIFG
        case 0x4: break; //TA0CCTL2.CCIFG
        case 0x6: break; //TA0CCTL3.CCIFG
        case 0x8: break; //TA0CCTL4.CCIFG
        case 0xA: break; //TA0CCTL5.CCIFG (Não existe no TA0)
        case 0xC: break; //TA0CCTL6.CCIFG (Não existe no TA0)
        case 0xE: //TA0CTL.TAIV
            timer_callback[TIMER_A0]();
        break;
    }
}

#pragma vector = TIMER1_A1_VECTOR
__interrupt void isr_ta1(void){
    int n;
    n = __even_in_range(TA1IV,0xE);
    switch(n) {
        case 0x0: break; //Nenhuma interrupção pendente
        case 0x2: break; //TA1CCTL1.CCIFG
        case 0x4: break; //TA1CCTL2.CCIFG
        case 0x6: break; //TA1CCTL3.CCIFG (Não existe no TA0)
        case 0x8: break; //TA1CCTL4.CCIFG (Não existe no TA0)
        case 0xA: break; //TA1CCTL5.CCIFG (Não existe no TA0)
        case 0xC: break; //TA1CCTL6.CCIFG (Não existe no TA0)
        case 0xE: //TA1CTL.TAIV
            timer_callback[TIMER_A1]();
        break;
    }
}
