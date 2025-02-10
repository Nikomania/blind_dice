/*
 * i2c.c
 *
 *  Created on: Jan 9, 2025
 *      Author: nikomania
 */
#include <i2c.h>

void init_i2c(void) {
    UCB0CTL1 |= UCSWRST;                    // Reseta para iniciar a configuração
    UCB0CTL0 = UCMST | UCMODE_3 | UCSYNC;   // Mestre, I2C (MODE = 3), síncrono
    UCB0CTL1 = UCSSEL_2 | UCSWRST;          // Usa SMCLK (1 MHz), mantém reset
    UCB0BR0 = 10;                           // SMCLK/10 = 100 kHz
    UCB0BR1 = 0;
    UCB0CTL1 &= ~UCSWRST;                   // Limpa reset para terminar a configuração
}

uint8_t i2c_test(uint8_t slave_addr) {
    __disable_interrupt();
    UCB0I2CSA = slave_addr;
    UCB0CTL1 |= UCTR;
    UCB0CTL1 |= UCTXSTT;
    while ((UCB0IFG & UCTXIFG) == 0);
    UCB0CTL1 |= UCTXSTP;
    while ((UCB0IFG & UCTXSTP) == UCTXSTP);
    const int result = (UCB0IFG & UCNACKIFG) == 0;
    __enable_interrupt();
    return result;
}

void i2c_send(uint8_t slave_addr, uint8_t register_addr, uint8_t data) {
    __disable_interrupt();
    // Aguarda qualquer transmissão em andamento finalizar
    while (UCB0CTL1 & UCTXSTP);

    // Define endereço do escravo
    UCB0I2CSA = slave_addr;

    // Limpa flags existentes
    UCB0IFG &= ~(UCNACKIFG | UCTXIFG);

    UCB0CTL1 |= UCTR | UCTXSTT; //Mestre TX + START
    while ( (UCB0IFG & UCTXIFG) == 0); //TXIFG=1?

    UCB0TXBUF = register_addr; //Escrever dado
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT); //Esperar STT=0
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG){ //NACK?
        P1OUT |= BIT0; //NACK=problema
        while(1); //Travar execução
    }
    while ( (UCB0IFG & UCTXIFG) == 0); //TXIFG=1?

    UCB0TXBUF = data; //Escrever dado
    while ( (UCB0IFG & UCTXIFG) == 0); //TXIFG=1?

    UCB0CTL1 |= UCTXSTP; //Gerar STOP
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP); //Esperar STOP

    __enable_interrupt();
}

void i2c_read(uint8_t slave_addr, uint8_t register_addr, uint8_t* byte) {
    __disable_interrupt();
    // Aguarda qualquer transmissão em andamento finalizar
    while (UCB0CTL1 & UCTXSTP);

    // Define endereço do escravo
    UCB0I2CSA = slave_addr;

    // Limpa flags existentes
    UCB0IFG &= ~(UCNACKIFG | UCTXIFG);

    UCB0CTL1 |= UCTR | UCTXSTT; //Mestre TX + Gerar START
    while ( (UCB0IFG & UCTXIFG) == 0); //Esperar TXIFG=1

    UCB0TXBUF = register_addr; //Escrever registrador
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT); //STT=0?
    if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG){ //NACK?
        P1OUT |= BIT0; //NACK=problema
        while(1); //Travar execução
    }

    // Configurar escravo transmissor
    UCB0CTL1 &= ~UCTR; //Mestre RX
    UCB0CTL1 |= UCTXSTT; //START Repetido
    while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT); //STT=0?

    UCB0CTL1 |= UCTXSTP; //Gerar STOP
    while ( (UCB0IFG & UCRXIFG) == 0); //Esperar RX
    while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP); //Esperar STOP

    *byte = UCB0RXBUF;
    __enable_interrupt();
}

void i2c_read_vect(uint8_t slave_addr, uint8_t register_addr, uint8_t byte_count, volatile uint8_t* data) {
    __disable_interrupt();
    // Aguarda qualquer transmissão em andamento finalizar
    while (UCB0CTL1 & UCTXSTP);

    // Define endereço do escravo
    UCB0I2CSA = slave_addr;

    // Limpa flags existentes
    UCB0IFG &= ~(UCNACKIFG | UCTXIFG);

     // Indicar registrador de onde começa a leitura
     UCB0CTL1 |= UCTR | UCTXSTT; //Mestre TX + Gerar START
     while ( (UCB0IFG & UCTXIFG) == 0); //Esperar TXIFG=1

     UCB0TXBUF = register_addr; //Escrever registrador
     while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT); //STT=0?
     if ( (UCB0IFG & UCNACKIFG) == UCNACKIFG){ //NACK?
         P1OUT |= BIT0; //NACK=problema
         while(1); //Travar execução
     }

     // Configurar escravo transmissor
     UCB0CTL1 &= ~UCTR; //Mestre RX
     UCB0CTL1 |= UCTXSTT; //START Repetido
     while ( (UCB0CTL1 & UCTXSTT) == UCTXSTT); //STT=0?

     // Ler a quantidade de dados, menos o último
     uint8_t i;
     for (i=0; i<byte_count-1; i++){
     while ((UCB0IFG & UCRXIFG) == 0); //Esperar RX
         data[i]=UCB0RXBUF; //Ler dado
     }

     // Ler o último dado e gerar STOP
     UCB0CTL1 |= UCTXSTP; //Gerar STOP
     while ((UCB0IFG & UCRXIFG) == 0); //Esperar RX

     data[i]=UCB0RXBUF; //Ler dado

     while ( (UCB0CTL1 & UCTXSTP) == UCTXSTP); //Esperar STOP

     __enable_interrupt();
}
