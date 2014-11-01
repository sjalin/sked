#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "lcd.h"
#include "system.h"

void InitSystem(void){

	//clock
	CLKPR &= 1 << CLKPCE; // CLKdiv = 0 
	CLKPR = 0; 

	// Timer 1 overflow compare, div 256
	TIMSK1 = 1 << TOIE0 | 1 << OCIE1A;
	OCR1A = 0x0001;
	TCNT1 = 0x00;
	TCCR1B = 1 << CS12 | 1 << CS10;  

	set_sleep_mode(SLEEP_MODE_IDLE);


	//warning: TODO: add watchdog
	/*
	WDTCR = (1 << WDCE); // Watchdog change enable
	WDTCR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);  // Watchdog enable, 2048k cykles
	*/
	
	sei();
}




