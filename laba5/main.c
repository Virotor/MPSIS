#include <msp430.h>
#include <math.h>
#include "screen_utils.h"

typedef unsigned char uchar;



#define NONE                        0
#define READ_X            0x18
#define READ_Y            0x1C
#define READ_Z            0x20



#define DELAY 1000

unsigned char whatChecking = 0;
unsigned char isRxReady = 0;
unsigned char isTxReady = 0;



long int CONVERT = 9.8*39.37/1000;
volatile long int DATA_FOR_TRANSLATE[] = { 4571, 2286, 1142, 571, 286, 143, 71 };

void Delay(long int value);

void CMA3000_init();
uchar cma3000_SPI(uchar byte_one, uchar byte_two);
long int getValueFromAcs(uchar projection_byte);
uint8_t CMA3000_writeCommand(uint8_t firstByte, uint8_t secondByte);

void startTimerA1(){
    TA1CCTL0 = CCIE;
    TA1CCR0 = 5240;
    TA1EX0 = TAIDEX_4;
    TA1CTL = TASSEL_2 | ID__4 | MC_1 | TACLR;
}

void stopTimerA1() {
    TA1CCTL0 &= ~CCIE;
    TA1CTL = MC_0;
}

int main(void)
{
  UCSCTL1 = DCORSEL_4;
  UCSCTL3 |= FLLREFDIV__1;
  UCSCTL2 = 274;
  UCSCTL2 |= FLLD__2;

  WDTCTL = WDTPW | WDTHOLD;

  P1DIR |= BIT4;
  P1OUT &= ~BIT4;


  screenInit();
  backlightInit();
  clearScreen();
  CMA3000_init();

  __bis_SR_register(GIE + LPM0_bits);
  __no_operation();

  return 0;
}



uint8_t CMA3000_writeCommand(uint8_t firstByte, uint8_t secondByte) {
    char miso_data;

    P3OUT &= ~BIT5;
    P2IE &= ~BIT5;

    miso_data = UCA0RXBUF;

    startTimerA1();

    whatChecking = 1;
    __bis_SR_register(LPM0_bits + GIE);
    UCA0TXBUF = firstByte;
    isTxReady = 0;

    whatChecking = 2;
    __bis_SR_register(LPM0_bits + GIE);
    miso_data = UCA0RXBUF;
    isRxReady = 0;

    whatChecking = 1;
    __bis_SR_register(LPM0_bits + GIE);
    UCA0TXBUF = secondByte;
    isTxReady = 0;

    whatChecking = 2;
    __bis_SR_register(LPM0_bits + GIE);
    miso_data = UCA0RXBUF;
    isRxReady = 0;

    stopTimerA1();
    P3OUT |= BIT5;
    P2IE |= BIT5;

    return miso_data;
}




void CMA3000_init(void) {
    P2DIR  &= ~BIT5;
    P2OUT  |=  BIT5;
    P2REN  |=  BIT5;
    P2IE   |=  BIT5;
    P2IES  &= ~BIT5;
    P2IFG  &= ~BIT5;
    P3DIR  |=  BIT5;
    P3OUT  |=  BIT5;
    P2DIR  |=  BIT7;
    P2SEL  |=  BIT7;
    P3DIR  |= (BIT3 | BIT6);
    P3DIR  &= ~BIT4;
    P3SEL  |= (BIT3 | BIT4);
    P3OUT  |= BIT6;
    UCA0CTL1 = UCSSEL_2 | UCSWRST;
    UCA0BR0 = 0x30;
    UCA0BR1 = 0x0;
    UCA0CTL0 = UCCKPH & ~UCCKPL | UCMSB | UCMST | UCSYNC | UCMODE_0;
    UCA0CTL1 &= ~UCSWRST;

    CMA3000_writeCommand(0x0A, BIT2 | BIT4);
    __delay_cycles(25000);
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void TIMER_1 (void) {
    if(whatChecking == 1)
    {
        if(UCA0IFG & UCTXIFG)
        {
            isTxReady = 1;
            __bic_SR_register_on_exit(LPM0_bits + GIE);
        }
    }
    else if(whatChecking == 2)
    {
        if(UCA0IFG & UCRXIFG)
        {
            isRxReady = 1;
            __bic_SR_register_on_exit(LPM0_bits + GIE);
        }
    }

}

inline long int parseProjectionByte(uint8_t projectionByte) {
    uint8_t BITx[] = { BIT6, BIT5, BIT4, BIT3, BIT2, BIT1, BIT0 };
    volatile long int i = 0;
    volatile long int projectionValue = 0;
    volatile long int isNegative = projectionByte & BIT7;
    for (; i < 7; i++) {
        if (isNegative) {
            projectionValue += (BITx[i] & projectionByte) ? 0 : DATA_FOR_TRANSLATE[i];
        }else{
            projectionValue += (BITx[i] & projectionByte) ? DATA_FOR_TRANSLATE[i] : 0;
        }
    }
    projectionValue *= isNegative ? -1 : 1;
    return projectionValue;
}

inline long double Angle(double z){
    return acos(z/1000);
}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ACCEL_ISR(void)
{
    volatile uint8_t zProjectionByte = CMA3000_writeCommand(READ_Z, NONE);
    volatile uint8_t xProjectionByte = CMA3000_writeCommand(READ_X, NONE);

    volatile long   int z = parseProjectionByte(zProjectionByte);
    volatile long   int x = parseProjectionByte(xProjectionByte);

    volatile unsigned int inchPerSecondSquaredMultiplied = abs(z)*9.8/1000*39.37*100;


    double angleZ =0;
    angleZ = Angle(z);
    angleZ *= 57.3 ;
    angleZ  +=90;
    clearScreen();
    printNumber(inchPerSecondSquaredMultiplied,z<0);
    if(150 <= angleZ && angleZ <= 210 && x<0 ){
        P1OUT |= BIT4;
    }else{
        P1OUT &= ~BIT4;
    }
}
