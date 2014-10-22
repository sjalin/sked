//#include <stdio.h>
#include <avr/io.h>

void LCD_Init(void);

void clcscreen(void);

void print_char(int num, int pos); 

//void print_temp(lcd_t *self, float *temp);

void debugprint(float temp1);

void printTime1(int test);

void print_temp2(float temp1);

void lcdshowtempoffset(float temp1);
