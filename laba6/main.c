#include <msp430.h>
#include "CTS_Layer.h"
#include <math.h>
#include "screen_utils.h"


void SetupTimer();


/* PADS ------------------------------------------------------------------------------------------------------ */

#define NUM_KEYS    5
#define LED4        BIT5
#define LED5        BIT4
#define LED6        BIT3
#define LED7        BIT2
#define LED8        BIT1

struct Element* keypressed;

const struct Element* address_list[NUM_KEYS] = {
        &PAD1,
        &PAD2,
        &PAD3,
        &PAD4,
        &PAD5
};
const uint8_t ledMask[NUM_KEYS] = {
LED4,
LED5,
LED6,
LED7,
LED8
};

void SetupTimer()
{
    TA0CCTL0 |= CCIE;
    TA0CTL = TASSEL__SMCLK | MC__UP | ID__1 | TACLR;
    long int second = 32768;
    long int period = second / 2;
    TA0CCR0 = second;
    TA0CCR1 = period;
    TA0CCTL1 = OUTMOD_3;
}

void Potentiometer_init() {
    P6DIR &= ~BIT5;
    P6SEL |= BIT5;
    P8DIR |= BIT0;
    P8SEL &= ~BIT0;
    P8OUT |= BIT0;
}





#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR() {

    TA0CTL &= ~MC__UP;
    TA0CTL |= MC__STOP | TACLR;
    TA0CCTL1 &= ~BIT2;

    clearScreen();
    printNumber(ADC12MEM0 * (1.5 / 4096) * 1000);
    ADC12CTL0 &= ~ADC12ENC;
}

void SetVcoreUp(uint16_t level)
{

    PMMCTL0_H = PMMPW_H;
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;
    while ((PMMIFG & SVSMLDLYIFG) == 0);
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);
    PMMCTL0_L = PMMCOREV0 * level;
    if ((PMMIFG & SVMLIFG))
        while ((PMMIFG & SVMLVLRIFG) == 0);
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;
    PMMCTL0_H = 0x00;
}
void SetupADC()
{
    ADC12CTL0 = ADC12SHT0_15 | ADC12ON;
    ADC12CTL1 &= ~ADC12SHP;
    ADC12CTL1 = ADC12CSTARTADD_0 |
            ADC12SHS_1 |
            ADC12SSEL_0 |
            ADC12CONSEQ_0;
    ADC12MCTL0 = ADC12EOS | ADC12INCH_5;
    ADC12IE = ADC12IE0;
}


void initTA(){
    TA1CCTL0 |= CCIE;
    TA1CTL = TASSEL__SMCLK | ID__8| MC__UP | TACLR;
    TA1CCR0 = 32700;
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void ta1_isr (void){
        P1OUT &= ~(LED4 + LED5 + LED6 + LED7 + LED8);
         keypressed = (struct Element *) TI_CAPT_Buttons(&keypad);
         if (keypressed == address_list[0])
             {

                 P1OUT |= BIT1;

                 if (!(ADC12CTL1 & ADC12BUSY))
                   {
                       SetupTimer();
                       ADC12CTL0 |= ADC12ENC;
                   }
             }

}

int main(void)
{
kkkj      WDTCTL = WDTPW | WDTHOLD;

    screenInit();
    backlightInit();
    clearScreen();

    P1DIR = 0xFF;
    P1OUT = 0;


    SetVcoreUp(0x01);
    SetVcoreUp(0x02);
    SetVcoreUp(0x03);

    UCSCTL3 = SELREF_2;
    UCSCTL4 |= SELA_2;

    __bis_SR_register(SCG0);
    UCSCTL0 = 0x0000;
    UCSCTL1 = DCORSEL_7;

    UCSCTL2 = FLLD_1 + 762;

    __bic_SR_register(SCG0);

    TI_CAPT_Init_Baseline(&keypad);
    TI_CAPT_Update_Baseline(&keypad, 5);

    Potentiometer_init();
    SetupADC();
    initTA();
    __bis_SR_register(GIE);
     _no_operation();

}

