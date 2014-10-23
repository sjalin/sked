#include <avr/interrupt.h>
#include "stuff.h"

uint32 time = 0;
uint32 timer1Overflow = 0; 
uint32 timeOfNextWake;

uint32 GetCurrentTime()
{
	time = (timer1Overflow*65535 + TCNT1)/TICKS_EVERY_MS;
    return time;
}

void TimeTick(){
   time++;
}
void SetNextWake(uint32 time)
{
	timeOfNextWake = time;		
	/*
	if (next time < nästa overflow)
		set OCR1A
		starta compare interrupt
	*/
}



ISR(TIMER1_COMPA_vect){
	//OCR1A = OCR1A << 1;
	//PORTB ^= 0xC;
	//Stäng av compare interrupt
}

ISR(TIMER1_OVF_vect){
	//OCR1A = 0x0001;
	/*
	if (next time < nästa overflow)
		set OCR1A
		starta compare interrupt
	*/
	PORTB ^= 0xFF0;
	timer1Overflow++;
}
