/*Includes*/
#include <avr/io.h>
#include <math.h>

#include "lcd.h"

/*
Define the diffrent segments
      a
    -----
   |     |
  f|  g  |b
   |-----|
   |     |
  e|     |c
    -----     
      d
bp is the dot or the doubble dot.
*/

#define a 0x40
#define b 0x20
#define c 0x10
#define d 0x8
#define e 0x4
#define f 0x2
#define g 0x1
#define bp 0x80 //Double dot on between segment 2 and 3 controlled by LCDDR04

#define MINUS 11
#define POINT 12

const char char_gen[] = {
 a+b+c+d+e+f,   // Displays "0"
 b+c,           // Displays "1"
 a+b+d+e+g,     // Displays "2"
 a+b+c+d+g,     // Displays "3"
 b+c+f+g,       // Displays "4"
 a+c+d+f+g,     // Displays "5"
 a+c+d+e+f+g,   // Displays "6"
 a+b+c,         // Displays "7"
 a+b+c+d+e+f+g, // Displays "8"
 a+b+c+d+f+g,   // Displays "9"
 a+f+e+d,       // Displays "C" (10)
 g,             // Displays "-" (11)
 bp,			// Displays "." (12)
 a+f+g+e+d,     // Displays "E" (13)
 a+f+g+e+c+b    // Displays "R" (14)
};

int buttonActive = 0; 

void InitLCD(void)
{
	/* Use 32 kHz crystal oscillator */
	/* Static Bias and Static duty, SEG21:SEG24 is used as port pins */

	//Static Bias and Static Duty
	LCDCRB &= ~(1<<LCDMUX0);
	LCDCRB &= ~(1<<LCDMUX1);
	
	//Use segment 0 till 39
	LCDCRB |= (1<<LCDPM0);
	LCDCRB |= (1<<LCDPM1);
	LCDCRB |= (1<<LCDPM2);
	LCDCRB |= (1<<LCDPM3);

	LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDPS2);	
	
	/* Maximum constrast*/
	LCDCCR = (1<<LCDMDT);
	
	// Turn display on, disable voltagebuffer (spares energy),LCD Low Power Waveform on.
	LCDCRA = (1<<LCDEN) | (1<<LCDBD) | (1<<LCDAB);
}

void clearScreen(void)
{
	clearSegment(0);	
	clearSegment(1);	
	clearSegment(2);	
	clearSegment(3);	
	clearSegment(4);	
}

void clearSegment(uint16 segment){
	switch (segment){
		case 0: 
			LCDDR00 = 0;
			break; 
		case 1: 
			LCDDR01 = 0;
			break; 
		case 2: 
			LCDDR02 = 0;
			break; 
		case 3: 
			LCDDR03 = 0;
			break; 
		case 4: 
			LCDDR04 = 0;
			break; 
		default:
			break; 
	}	
}


/*Prints a token that can be found in char_gen at the lcd-pos pos.*/
void print_char(int num, int pos)
{	
	if(pos >= 0 && pos <= 4){
		if (pos == 0){
			LCDDR00 = char_gen[num];
		}else if (pos == 1){
			LCDDR01 = char_gen[num];
		}else if (pos == 2){
			LCDDR02 = char_gen[num];
		}else if (pos == 3){
			LCDDR03 = char_gen[num];
		}else if (pos == 4){
			LCDDR04 = char_gen[num];
		}
	}else{
		LCDDR00 = a+f+e+g+d;
		LCDDR01 = char_gen[0];
		LCDDR02 = char_gen[1];
	}
}

void PrintFloat(float number)
{
	//uint16 nr; 
	//LCDDR00 = bp;
	//LCDDR01 = bp;
	//LCDDR02 = bp;
	//LCDDR03 = bp;
	//LCDDR04 = bp; 
	
	if (number < 0){
		LCDDR00 = char_gen[MINUS];
		number = number*(-1); 
	}else{
		LCDDR00 = 0; 
	}


	number *= 100;
	
	LCDDR04 = char_gen[((uint16) number)%10];
	LCDDR03 = char_gen[(((uint16) number)/10)%10];
	LCDDR02 = char_gen[(((uint16) number)/100)%10] +  char_gen[POINT];
	if ((((uint16) number)/1000)%10){
		LCDDR01 = char_gen[(((uint16) number)/1000)%10];
	}else{
		LCDDR01 = 0; 
	}
}




void printTemp(float temp1)
{
	int Error = 0;
	float temp = temp1;// *temp1;
	int temp2;
	int firstdigit = 0;
	int seconddigit = 0;
	int fract = 0;

	clearScreen(); // Clears the screen

	if ((-99.9 <= temp) && (temp <= 99.9)){
		if (temp<0){		// checks if temp is a negativ number
			LCDDR00 = char_gen[11];
			temp = temp * (-1);
		}

		temp = temp * 10;
		temp = round(temp);
		temp2 = (int)temp;

		int inc = 0;
		while(temp2!=0){
			if(inc == 0){
				fract = temp2%10;
			}else if(inc == 1){
				seconddigit = (temp2%10);
			}else if(inc == 2){
				firstdigit = (temp2%10);
			}

			inc++;
			temp2 = temp2/10;
		}
		
		if(firstdigit == 0){
			if(seconddigit < 13 && fract < 13){
				LCDDR01 = char_gen[seconddigit] + char_gen[12];
				LCDDR02 = char_gen[fract];
				LCDDR03 = char_gen[10];
			}else{
				Error = 1;
			}
		}else if(firstdigit != 0){
			if(firstdigit < 13 && seconddigit < 13 && fract < 13){
				LCDDR01 = char_gen[firstdigit];
				LCDDR02 = char_gen[seconddigit]+char_gen[12];
				LCDDR03 = char_gen[fract];
				LCDDR04 = char_gen[10];
			}else{
				Error = 1;
			}
		} 

	}

	if(Error){
		LCDDR00 = char_gen[13];
		LCDDR01 = char_gen[14];
		LCDDR02 = char_gen[14];
		LCDDR03 = char_gen[0];
		LCDDR04 = char_gen[14];
	}
}


void printNumber(uint16 number)
{
	clearScreen();
	print_char(number%10,4);
	print_char((number/10)%10,3);
	print_char((number/100)%10,2);
	print_char((number/1000)%10,1);
	print_char((number/10000)%10,0);
}
