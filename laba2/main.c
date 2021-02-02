#include <msp430.h> 

/*
 * main.c
 */

const int timeD = 6000;
int second = 32767;

int bDelay = 0;


 int flagS1 = 0;
 int flagS2 = 0;

 void delay(int value){
     TA1CTL=TASSEL_1|ID__1|MC__UP|TACLR;
     TA1CCR0 = value;
     TA1CCTL0 |= CCIE;
     bDelay= 1;
 }

#pragma vector = TIMER1_A0_VECTOR
__interrupt void ta1_isr (void){
    bDelay = 0;
    TA1CTL = TACLR;
}



#pragma vector = PORT1_VECTOR
__interrupt void InterruptButtonS1(void){

    if(bDelay==0){
        delay(timeD);
    if(flagS1==0){
        __bis_SR_register_on_exit ( CPUOFF| SCG0 );
        flagS1=1;
    }
    else if(flagS1==1){
        __bic_SR_register_on_exit ( CPUOFF| SCG0 );
        flagS1=0;

    }
    }

      P1IFG &=~BIT7;
}

#pragma vector = PORT2_VECTOR
__interrupt void InterruptButtonS2(void){

    if(bDelay==0){
        delay(timeD);
    if(flagS2==0)
    {
        UCSCTL4 &=~SELM__XT1CLK;
        UCSCTL4 &=SELM__VLOCLK;
        UCSCTL5 |= DIVM__16;
        UCSCTL5 &=~ DIVM__8;
        flagS2=1;
    }
    else if(flagS2==1)
    {
        //UCSCTL3 |=SELM__XT1CLK;
        UCSCTL4 &=~SELM__XT1CLK;
        UCSCTL4 |=SELM__DCOCLK;
        //UCSCTL5 =DIVM__8;
        UCSCTL5 &=~ DIVM__8;
        UCSCTL5 |= DIVM__16;
        flagS2=0;

    }
    }

      P2IFG &=~BIT2;
}



void setOut()
{

    P1OUT |= BIT7;
    P2OUT |= BIT2;
    P1REN |= BIT7;
    P2REN |= BIT2;

    P1IE  |= BIT7;

    P2IE  |= BIT2;

    P7SEL |=BIT7;
    P7DIR |=BIT7;

    UCSCTL3|=SELREF__XT1CLK;
    UCSCTL4|=SELM__DCOCLK;
    P1IFG &=~BIT7;
    P2IFG &=~BIT2;
}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    setOut();
    UCSCTL4 |= SELS__REFOCLK;

    UCSCTL1|=DCORSEL_1;


    UCSCTL3 |= FLLREFDIV__16;
    UCSCTL2 = FLLD__8 | 245;
    UCSCTL5 |= DIVM__2;



    __bis_SR_register(BIT3);

    while(1){
        __no_operation();
    }

}
