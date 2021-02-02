#include "screen_utils.h"

#define SYMBOL_HEIGHT 13

static const uint8_t zero[SYMBOL_HEIGHT]  = {0x7c, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c};
static const uint8_t one[SYMBOL_HEIGHT]   = {0xfe, 0xfe, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xf8, 0x78, 0x38, 0x18};
static const uint8_t two[SYMBOL_HEIGHT]   = {0xfe, 0xfe, 0xc0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x06, 0xc6, 0xc6, 0x7c, 0x38};
static const uint8_t three[SYMBOL_HEIGHT] = {0xfc, 0xfe, 0x06, 0x06, 0x06, 0x06, 0x7e, 0x7c, 0x06, 0x06, 0x06, 0xfe, 0xfc};
static const uint8_t four[SYMBOL_HEIGHT]	 = {0x06, 0x06, 0x06, 0x06, 0xfe, 0xfe, 0xc6, 0xc6, 0xe6, 0x7e, 0x3e, 0x1e, 0x0e};
static const uint8_t five[SYMBOL_HEIGHT]	 = {0xf8, 0xfc, 0x0e, 0x06, 0x06, 0x06, 0xfe, 0xfc, 0xc0, 0xc0, 0xc0, 0xfe, 0xfe};
static const uint8_t six[SYMBOL_HEIGHT] 	 = {0x7c, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0xfc, 0xc0, 0xc0, 0xc0, 0xfe, 0x7e};
static const uint8_t seven[SYMBOL_HEIGHT] = {0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x06, 0x06, 0xfe, 0xfe};
static const uint8_t eight[SYMBOL_HEIGHT] = {0x7c, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c};
static const uint8_t nine[SYMBOL_HEIGHT]  = {0x7c, 0x7e, 0x06, 0x06, 0x06, 0x7e, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xfe, 0x7c};
static const uint8_t plus[SYMBOL_HEIGHT]	 = {0x00, 0x00, 0x00, 0x30, 0x30, 0xfc, 0xfc, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00};
static const uint8_t minus[SYMBOL_HEIGHT] = {0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void screenInit()
{
    // Port initialization for LCD operation
    P5DIR |= BIT7;
    // Reset is active low
    P5OUT &= BIT7;
    // Reset is active low
    P5OUT |= BIT7;
    // Chip select for LCD
    P7DIR |= BIT4;
    // CS is active low
    P7OUT &= ~BIT4;
    // Command/Data for LCD
    P5DIR |= BIT6;
    // CD Low for command
    P5OUT &= ~BIT6;

    // P4.1 option select SIMO
    P4SEL |= BIT1;
    P4DIR |= BIT1;
    // P4.3 option select CLK
    P4SEL |= BIT3;
    P4DIR |= BIT3;

    // Initialize USCI_B1 for SPI Master operation
    // Put state machine in reset
    UCB1CTL1 |= UCSWRST;
    //3-pin, 8-bit SPI master
    UCB1CTL0 = UCCKPH + UCMSB + UCMST + UCMODE_0 + UCSYNC;
    // Clock phase - data captured first edge, change second edge
    // MSB
    // Use SMCLK, keep RESET
    UCB1CTL1 = UCSSEL_2 + UCSWRST;
    UCB1BR0 = 0x02;
    UCB1BR1 = 0;
    // Release USCI state machine
    UCB1CTL1 &= ~UCSWRST;
    UCB1IFG &= ~UCRXIFG;

#define SET_COLUMN_ADDRESS_MSB        0x10  //Set SRAM col. addr. before write, last 4 bits =
                                            // ca4-ca7
#define SET_COLUMN_ADDRESS_LSB        0x00  //Set SRAM col. addr. before write, last 4 bits =
                                            // ca0-ca3
#define SET_POWER_CONTROL             0x2F  //Set Power control - booster, regulator, and follower
                                            // on
#define SET_SCROLL_LINE               0x40  //Scroll image up by SL rows (SL = last 5 bits),
                                            // range:0-63
#define SET_PAGE_ADDRESS              0xB0  //Set SRAM page addr (pa = last 4 bits), range:0-8
#define SET_VLCD_RESISTOR_RATIO       0x27  //Set internal resistor ratio Rb/Ra to adjust contrast
#define SET_ELECTRONIC_VOLUME_MSB     0x81  //Set Electronic Volume "PM" to adjust contrast
#define SET_ELECTRONIC_VOLUME_LSB     0x0F  //Set Electronic Volume "PM" to adjust contrast (PM =
                                            // last 5 bits)
#define SET_ALL_PIXEL_ON              0xA4  //Disable all pixel on (last bit 1 to turn on all pixels
                                            // - does not affect memory)
#define SET_INVERSE_DISPLAY           0xA6  //Inverse display off (last bit 1 to invert display -
                                            // does not affect memory)
#define SET_DISPLAY_ENABLE            0xAF  //Enable display (exit sleep mode & restore power)
#define SET_SEG_DIRECTION             0xA1  //Mirror SEG (column) mapping (set bit0 to mirror
                                            // display)
#define SET_COM_DIRECTION             0xC8  //Mirror COM (row) mapping (set bit3 to mirror display)
#define SYSTEM_RESET                  0xE2  //Reset the system. Control regs reset, memory not
                                            // affected
#define NOP                           0xE3  //No operation
#define SET_LCD_BIAS_RATIO            0xA2  //Set voltage bias ratio (BR = bit0)
#define SET_CURSOR_UPDATE_MODE        0xE0  //Column address will increment with write operation
                                            // (but no wrap around)
#define RESET_CURSOR_UPDATE_MODE      0xEE  //Return cursor to column address from before cursor
                                            // update mode was set
#define SET_ADV_PROGRAM_CONTROL0_MSB  0xFA  //Set temp. compensation curve to -0.11%/C
#define SET_ADV_PROGRAM_CONTROL0_LSB  0x90

    uint8_t Dogs102x6_initMacro[] = {
        SET_SCROLL_LINE,
        SET_SEG_DIRECTION,
        SET_COM_DIRECTION,
        SET_ALL_PIXEL_ON,
        SET_INVERSE_DISPLAY,
        SET_LCD_BIAS_RATIO,
        SET_POWER_CONTROL,
        SET_VLCD_RESISTOR_RATIO,
        SET_ELECTRONIC_VOLUME_MSB,
        SET_ELECTRONIC_VOLUME_LSB,
        SET_ADV_PROGRAM_CONTROL0_MSB,
        SET_ADV_PROGRAM_CONTROL0_LSB,
        SET_DISPLAY_ENABLE,
        SET_PAGE_ADDRESS,
        SET_COLUMN_ADDRESS_MSB,
        SET_COLUMN_ADDRESS_LSB
    };

    Dogs102x6_writeCommand(Dogs102x6_initMacro, 13);

    dogs102x6Memory[0] = 102;
        dogs102x6Memory[1] = 8;


    // Deselect chip
    P7OUT |= BIT4;
}

void backlightInit()
{
    P7DIR |= BIT6;
    P7OUT |= BIT6;
}

void setRow(uint8_t row)
{
#define setPage (row&0x0F)|0xB0

	uint8_t cmd = setPage;
	Dogs102x6_writeCommand(&cmd, 1);
}

void setCol(uint8_t col)
{
#define setColL col&0x0F
#define	setColH ((col&0xF0) >> 4) | BIT4

	uint8_t cmd[2];
	cmd[0] = setColL;
	cmd[1] = setColH;

	Dogs102x6_writeCommand(cmd, 2);
}

void clearScreen()
{
    uint8_t LcdData[] = {0x00};
    uint8_t p, c;

    for (p = 0; p < 8; p++)
    {
        setRow(p);
        setCol(0);
        for (c = 0; c < 132; c++)
        {
            setCol(c);
            Dogs102x6_writeData(LcdData, 1);
        }
    }
}

void setMirroring(int mirror)
{
#define setSEG 0xa0|mirror;
#define	setCOM 0xc8;

	uint8_t cmd[2];
	cmd[0] = setSEG;
	cmd[1] = setCOM;
	Dogs102x6_writeCommand(cmd, 2);
}

void writeChar(uint8_t col, uint8_t row, char a)
{
	setCol(col);
	setRow(row);

	const uint8_t* c = zero;

	switch (a)
	{
	case '0':
		{
			c = zero;
			break;
		}
	case '1':
		{
			c = one;
			break;
		}
	case '2':
		{
			c = two;
			break;
		}

	case '3':
		{
			c = three;
			break;
		}
	case '4':
		{
			c = four;
			break;
		}
	case '5':
		{
			c = five;
			break;
		}
	case '6':
		{
			c = six;
			break;
		}
	case '7':
		{
			c = seven;
			break;
		}
	case '8':
		{
			c = eight;
			break;
		}
	case '9':
		{
			c = nine;
			break;
		}
	case '+':
		{
			c = plus;
			break;
		}
	case '-':
		{
			c = minus;
			break;
		}

	default:
		{
			break;
		}
	}
	Dogs102x6_writeData(c, SYMBOL_HEIGHT);
}

void writeString(uint8_t col, uint8_t row, char* s)
{
	int pos;
	for(pos = 0; *s != 0; ++pos, ++s)
	{
		writeChar(col, 7 - pos - row, *s);
	}
}

void printNumber(unsigned int number, int mirroring)
{
	char s[9];
	itoa(number, s, mirroring);
	clearScreen();
	writeString(0, 0, s);
}
