#ifndef SCREEN_UTILS_H
#define SCREEN_UTILS_H

#include <msp430.h>
#include "HAL_Dogs102x6.h"
#include "utils.h"

void screenInit();
void backlightInit();
void setRow(uint8_t row);
void setCol(uint8_t col);
void clearScreen();
void setMirroring(int mirror);
void writeChar(uint8_t col, uint8_t row, char a);
void writeString(uint8_t col, uint8_t row, char* s);
void printNumber(int number, int mirroring);

#endif
