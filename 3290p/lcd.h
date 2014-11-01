#ifndef LCD_H
#define LCD_H

#include "stuff.h"

void InitLCD(void);

void clearScreen(void);

void clearSegment(uint16 segment);

void print_char(int num, int pos); 

void printTemp(float temp1);

void printNumber(uint16 number);

void PrintFloat(float number); 

#endif
