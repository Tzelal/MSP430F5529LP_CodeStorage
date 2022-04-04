#include <msp430.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int force;
char ch[10];

void uart_write(char* str);

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

    P3SEL |= BIT3+BIT4;                       // P3.3,4 = USCI_A0 TXD/RXD
    UCA0CTL1 |= UCSWRST;                      // **Put state machine in reset**
    UCA0CTL1 |= UCSSEL_2;                     // SMCLK
    UCA0BR0 = 9;                              // 1MHz 115200 (see User's Guide)
    UCA0BR1 = 0;                              // 1MHz 115200
    UCA0MCTL |= UCBRS_1 + UCBRF_0;            // Modulation UCBRSx=1, UCBRFx=0
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt
    
    ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 enable
    ADC12CTL1 = ADC12SHP;                     // sampling timer
    ADC12IE = 0x01;                           // Enable interrupt
    ADC12CTL0 |= ADC12ENC;
    P6SEL |= 0x01;                            // P6.0 ADC
    P1DIR |= 0x01;  

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, interrupts enabled
    __no_operation();                         // For debugger

    
}

// Echo back RXed character, confirm TX buffer is ready first
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    while (1)
    {

        ADC12CTL0 |= ADC12SC;                   // Start sampling
        force=ADC12MEM0;
        ltoa(force, ch, 10);
        uart_write(ch);
        uart_write("\n");

        __delay_cycles(1000000);


        if(force>=3880)    //This value depends on the input voltage and shunt resistance
        P1OUT |= BIT0;
        else
        P1OUT &= ~BIT0;

    }
}


void uart_write(char* str) {
  int i;
  for(i = 0; str[i] != '\0'; i++) {
    while (!(UCA0IFG & UCTXIFG));    // TX buffer ready?
    UCA0TXBUF = str[i];
  }
}
