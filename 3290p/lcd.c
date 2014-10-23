/*Includes*/
#include "lcd.h"
#include <math.h>

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
#define bp 0x80

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

void LCD_Init(void)
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

void clcscreen(void)
{
	LCDDR00 = 0;
	LCDDR01 = 0;
	LCDDR02 = 0;
	LCDDR03 = 0;
	LCDDR04 = 0;
}


/*Prints a token that can be found in char_gen at the lcd-pos pos.*/
void print_char(int num, int pos)
{	
	if(pos >= 0 && pos <= 4){
			if (pos == 0){
				LCDDR00 = LCDDR00 + char_gen[num];
			}else if (pos == 1){
				LCDDR01 =  LCDDR01 + char_gen[num];
			}else if (pos == 2){
				LCDDR02 = LCDDR02 + char_gen[num];
			}else if (pos == 3){
				LCDDR03 = LCDDR03 + char_gen[num];
			}else if (pos == 4){
				LCDDR04 = LCDDR04 + char_gen[num];
			}
	}else{
		LCDDR00 = a+f+e+g+d;
		LCDDR01 = char_gen[0];
		LCDDR02 = char_gen[1];
	}
}

void print_temp2(float temp1)
{
	int Error = 0;
	float temp = temp1;// *temp1;
	int temp2;
	int firstdigit = 0;
	int seconddigit = 0;
	int fract = 0;

	clcscreen(); // Clears the screen

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

void lcdshowtempoffset(float temp1)
{
	int Error = 0;
	float temp = temp1;// *temp1;
	int temp2;
	int firstdigit = 0;
	int seconddigit = 0;
	int fract = 0;

	clcscreen(); // Clears the screen
	if ((-99.9 <= temp) && (temp <= 99.9)) 
	{
		if (temp<0)		// checks if temp is a negativ number
		{
			LCDDR00 = char_gen[11];
			temp = temp * (-1);
		}

		temp = temp * 10;
		temp = round(temp);
		temp2 = (int)temp;
	

		int inc = 0;
		while(temp2!=0)
		{

			if(inc == 0)
			{
				fract = temp2%10;
			
			}
			else if(inc == 1)
			{
				seconddigit = (temp2%10);

			}

			else if(inc == 2)
			{
				firstdigit = (temp2%10);
			}

			inc++;
			temp2 = temp2/10;

		}
	


		if(firstdigit == 0)
		{

			if(seconddigit < 13 && fract < 13)
			{
				LCDDR01 = char_gen[seconddigit] + char_gen[12];
				LCDDR02 = char_gen[fract];
				LCDDR03 = char_gen[10];
			}
			else
			{
				Error = 1;
			}
		}
		else if(firstdigit != 0)
		{

			if(firstdigit < 13 && seconddigit < 13 && fract < 13)
			{

				LCDDR01 = char_gen[firstdigit];
				LCDDR02 = char_gen[seconddigit]+char_gen[12];
				LCDDR03 = char_gen[fract];
				LCDDR04 = char_gen[10];
			}
			else
			{
				Error = 1;
			}
		} 

	}
	else
	{
		Error = 1;
	}

	if(Error)
	{
		LCDDR00 = char_gen[13];
		LCDDR01 = char_gen[14];
		LCDDR02 = char_gen[14];
		LCDDR03 = char_gen[0];
		LCDDR04 = char_gen[14];
	}

}


void debugprint(float temp1)
{
	clcscreen();
	int Error = 0;
	int firstD = 0;
	int secondD = 0;
	int thirdD = 0;
	int fourthD= 0;
	int inc = 0;
	int temp;

	temp1 = temp1 * 10;

	temp = (int)temp1;

	if(temp < 9999 && temp > - 9999)
	{
		if (temp<0)		// checks if temp is a negativ number
		{
			LCDDR00 = char_gen[11];
			temp = temp * (-1);
		}

		if (temp == 0)
		{
			LCDDR01 = char_gen[13];
			LCDDR02 = char_gen[0];
			LCDDR03 = char_gen[0];
			LCDDR04 = char_gen[0];
		}



		while(temp!=0)
		{
			if(inc == 0)
			{
				fourthD = (temp % 10);
			}

			else if(inc == 1)
			{
				thirdD = (temp % 10);
			}
			else if(inc == 2)
			{
				secondD = (temp % 10);
			}
			else if(inc == 3)
			{
				firstD = (temp % 10);
			}

			inc++;
			temp = temp / 10;

		}
	

		LCDDR01 = char_gen[firstD];
		LCDDR02 = char_gen[secondD];
		LCDDR03 = char_gen[thirdD];
		LCDDR04 = char_gen[fourthD];
	
	}
	
	else
	{
		Error = 1;
	}	


	if(Error)
	{
		clcscreen();
		LCDDR01 = char_gen[0];
		LCDDR02 = char_gen[0];
		LCDDR03 = char_gen[13];
		LCDDR04 = char_gen[13];
	}

}

	
void printTime1(int test)
{
	int inc = 0;
	int firstD = 0;
	int secondD = 0;
	int thirdD = 0;
	int fourthD = 0;
	int fifthD = 0;


	int temp = test;	

		while(temp!=0)
		{
			if(inc == 0)
			{
				fifthD = (temp % 10);
			}

			else if(inc == 1)
			{
				fourthD = (temp % 10);
			}
			else if(inc == 2)
			{
				thirdD = (temp % 10);
			}
			else if(inc == 3)
			{
				secondD = (temp % 10);
			}
			else if(inc == 3)
			{
				firstD = (temp % 10);
			}
			inc++;
			temp = temp / 10;
		}
	
		LCDDR00 = char_gen[firstD];
		LCDDR01 = char_gen[secondD];
		LCDDR02 = char_gen[thirdD];
		LCDDR03 = char_gen[fourthD];
		LCDDR04 = char_gen[fifthD];
}
