#include <msp430.h>
#include <i2c.h>
#include <gpio.h>
#include <stdint.h>

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    init_gpio();

    initI2C_Master();                       // Inicializa UCB0 como mestre

    if (i2cTest(ADXL345_ADDR) == 0) {
        P1OUT |= BIT0;
        while(1);
    }

    uint8_t devid;
    // check devid of ADXL345
    if (!i2cRead(ADXL345_ADDR, ADXL345_DEVID_ADDR, &devid) || devid != ADXL345_DEVID_VAL) {
        P1OUT |= BIT0;
        while(1);
    }

    i2cADXL345Config();

    volatile uint8_t data[ADXL345_BYTES_SENT];
    volatile int x, y, z;
    volatile int pressed = 0, was_pressed = 0;

    // Loop principal
    for(;;) {
        pressed = is_button_pressed();
        // Envia dados de teste e verifica o reconhecimento
        if(i2cReadVect(ADXL345_ADDR, ADXL345_DATA_ADDR, ADXL345_BYTES_SENT, data)) {
            // Transmissão bem-sucedida
            P4OUT |= GREEN_LED;
            x = data[0] | (data[1] << 8);
            y = data[2] | (data[3] << 8);
            z = data[4] | (data[5] << 8);
        } else {
            // Transmissão falhou
            P1OUT |= RED_LED;
            x = 0xFFFF;
            y = 0xFFFF;
            z = 0xFFFF;
        }
        if (pressed && !was_pressed) {
            buzzer_toggle();
            was_pressed = 1;
            __delay_cycles(100000);
        } else if (!pressed) {
            was_pressed = 0;
        }
        __delay_cycles(100000);
        P4OUT &= ~GREEN_LED;
        P1OUT &= ~RED_LED;
        __delay_cycles(100000);
    }
}
