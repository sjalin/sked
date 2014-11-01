#include <avr/io.h>
#include "debug.h"


/*ISR(PCINT2_vect)
{
	PORTB ^= 0xFF;//	blinkLed(4);
}*/

void BlinkLed(uint16 poss)
{
	PORTB ^= poss; 
}



void InitDebug()
{
	//Ports
	DDRB = 0xFF;
//	DDRH = 0x00;
	PORTB = 0xFF;
//	PORTH = 0x55;


	//Port h interrupt 
//	EICRA = 1 << ISC01 | 1 << ISC00;
//	EIMSK = /*1 << INT0 |*/ 0x40;
//	PCMSK2 = 0xFF;
}
