#include <msp430.h>
#include "screen_utils.h"

/*
 * main.c
 */

int number = -7863;
int increment = 781;
int mirroring = 1;

#pragma vector = PORT1_VECTOR
__interrupt void S1_ROUTINE(void)
{
    number += increment;
    printNumber(number, mirroring);
    P1IFG &= ~BIT7;
}

#pragma vector = PORT2_VECTOR
__interrupt void S2_ROUTINE(void)
{
    mirroring ^= BIT0;
    setMirroring(mirroring);
    printNumber(number, mirroring);
    P2IFG &= ~BIT2;
}

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //s1
    P1DIR &= ~BIT7;
    P1SEL &= ~BIT7;
    P1REN |= BIT7;
    P1OUT |= BIT7;
    P1IE |= BIT7;
    P1IES |= BIT7;
    P1IFG &= ~BIT7;

    //s2
    P2DIR &= ~BIT2;
    P2SEL &= ~BIT2;
    P2REN |= BIT2;
    P2OUT |= BIT2;
    P2IE |= BIT2;
    P2IES |= BIT2;
    P2IFG &= ~BIT2;

    screenInit();
    backlightInit();
    clearScreen();

    setMirroring(mirroring);
    printNumber(number, mirroring);

    __bis_SR_register(GIE);
    return 0;
}
