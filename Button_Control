#include <msp430.h> 

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1DIR |=BIT0;
    P2REN |=BIT1;
    P2OUT |=BIT1; //Pull-Up resistor when pressed logic val 0.
    while(1)
    {
        if((P2IN & BIT1)==0) // BIT1 == 00000010, P2IN pull-up when pressed (0&1 == 0) 0 == 0 so true. 
        {
            P1OUT |=BIT0;
        }
        else
        {
            P1OUT &=~BIT0;
        }
    }
}
