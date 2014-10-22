#include <avr/interrupt.h>
#include <avr/io.h>

#include "lcd.h"
#include "sched.h"
#include "system.h"


//TODO: make all functions the same cameCase standard
//TODO: add leds to card to make debugging 


void blinkLed(uint16 arg)
{
	//The blink
	PORTB ^= 0x30;
}

ISR(PCINT2_vect)
{
	PORTB ^= 0xFF;//	blinkLed(4);
}

void print_first(uint16 nr)
{	
	static int x = 0x1;
	x ^= 0x3;
	clcscreen();
	print_char(x,1);
}
void print_second(uint16 nr)
{
	static int x = 0x1;
	x ^= 0x3;

	clcscreen();
	print_char(x,2);
}


int main()
{
	initSystem();
	InitScheduler();
	AddEvent(&print_first, (uint16) MEDIUM, MSEC(100), CYCLIC, 1);  
	//AddEvent(&print_second, (uint16) MEDIUM, MSEC(500), CYCLIC, 2);
	//AddEvent(&print_first, MEDIUM, MSEC(876), CYCLIC, 3);

	StartScheduler();
	
	return 0; 
}


