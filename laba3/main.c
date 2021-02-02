#include <msp430.h>



int selected_timer = 0;
int selected_del = 1;
int period;
int counter = 0;
int count = 0;
int count_wdt =0;
int canChange = 1;
const int timeD = 6000;
int second = 32767;

int bDelay = 0;


void setButton(){

    _disable_interrupts();
    P1DIR &= ~BIT7;
    P1SEL &= ~BIT7;
    P1REN |= BIT7;
    P1OUT |= BIT7;
    P1IE |= BIT7;
    P1IFG &= ~BIT7;

    P2DIR &= ~BIT2;
    P2SEL &= ~BIT2;
    P2REN |= BIT2;
    P2OUT |= BIT2;
    P2IE |= BIT2;;
    P2IFG &= ~BIT2;

    _enable_interrupts();
    P8DIR |= BIT1;
    P8OUT &=~ BIT1;

    P1DIR |= BIT2;
    P1OUT |= BIT2;

    P1DIR |= BIT3;
    P1OUT |= BIT3;

    P1DIR |= BIT4;
    P1OUT |= BIT4;


    P1DIR |= BIT5;  // set led 5 writing
    P1OUT &=~ BIT5; // set led 5 inactive level
    P1SEL |= BIT5;  // set led 5 to work with periphery

}


void set_smclk(){


    UCSCTL4 |= SELS__REFOCLK;
    UCSCTL4 |= SELA__REFOCLK;
}

void initTA(){
    TA2CCTL0 |= CCIE;
    TA2CTL = TASSEL__SMCLK | ID__2 | MC__UP | TACLR;
    TA2CCR0 = 31500;
}
void blink(void){
    switch(counter)
    {
    case 0:
        P1OUT &= ~BIT2;
        counter++;
        break;
    case 1:
        P1OUT &= ~BIT3;
        counter++;
        break;
    case 2:
        P1OUT &= ~BIT4;
        counter++;
        break;
    case 3:
        P1OUT |= BIT2;
        P1OUT |= BIT3;
        P1OUT |= BIT4;
        counter = 0;
        break;
    default:
        break;
    }
}


void delay(int value){
    TA1CTL=TASSEL_1|ID__1|MC__UP|TACLR;
    TA1CCR0 = value;
    TA1CCTL0 |= CCIE;
    bDelay= 1;
}


#pragma vector = WDT_VECTOR
interrupt void wdt_isr(void){
    if( selected_timer == 1 && count_wdt == 140){
            blink();
            count_wdt=0;
        }
    else{
        count_wdt++;
    }
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void ta2_isr (void){
    if(selected_timer == 0 && count == 16){
        blink();
        count=0;
    }
    else{
        count++;
    }
}




#pragma vector = PORT1_VECTOR
__interrupt void button_s1_isr(void){
   if(bDelay == 0){
       delay(timeD);
       if(canChange == 1){
           if(selected_timer == 0){
               TA2CCTL0 &= ~CCIE;
               TA2CCTL0 &= ~OUT;
           }
           else{
               WDTCTL = WDTPW + WDTHOLD;
           }
           canChange = 0;
           P1IES &=~ BIT7;
       }
       else {
          if(selected_timer==0){
              initTA();
          }
          else{
              SFRIE1 |= WDTIE;
              WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL__SMCLK | WDTIS_5;
          }
          canChange = 1;
          P1IES |= BIT7;
       }
   }
   P1IFG &= ~BIT7;
}

#pragma vector = PORT2_VECTOR
interrupt void button_s2_isr (void){
    if(bDelay== 0){
        delay(timeD);
        if(canChange == 1){
             if (selected_timer == 0){
                     TA2CCTL0 &= ~CCIE;
                     TA2CCTL0 &= ~OUT;
                     SFRIE1 |= WDTIE;
                     WDTCTL = WDTPW | WDTTMSEL | WDTCNTCL | WDTSSEL__SMCLK | WDTIS_5;
                     P8OUT ^= BIT1;
                     selected_timer = 1;
                 }
                 else if (selected_timer ==  1){
                     WDTCTL = WDTPW + WDTHOLD;
                     selected_timer = 0;
                     P8OUT ^= BIT1;
                     initTA();
                 }
         }
         else{
             if(selected_timer == 1){
                 selected_timer=0;
             }
             else{
                 selected_timer =1;
             }
         }
    }
    P2IFG &= ~BIT2;
}



#pragma vector = TIMER1_A0_VECTOR
__interrupt void ta1_isr (void){
    bDelay = 0;
}





int main(void){
    WDTCTL = WDTPW + WDTHOLD;

    setButton();

    set_smclk();

    TA0CTL = TASSEL_1 | ID__2 | MC__UP | TACLR;
    volatile long int period = (long int)(33800);
    TA0CCR0 = period;
    TA0CCR4 = (long int)(27160);
    TA0CCTL4 |= (OUTMOD_3);

    initTA();

    _bis_SR_register(GIE);

    _no_operation();
    return 0;
}
