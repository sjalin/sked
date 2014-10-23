#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "lcd.h"
#include "system.h"

void initSystem(void){

	//clock
	CLKPR &= 1 << CLKPCE; // CLKdiv = 0 
	CLKPR = 0; 

	// Timer 1 overflow compare, div 256
	TIMSK1 = 1 << TOIE0 | 1 << OCIE1A;
	OCR1A = 0x0001;
	TCNT1 = 0x00;
	TCCR1B = /*1 << CS12 |*/ 1 << CS10;  

	//Ports
	DDRB = 0xFF;
	DDRH = 0x00;
	PORTB = 0x00;
	PORTH = 0x55;
	set_sleep_mode(SLEEP_MODE_IDLE);

	//Port h interrupt 
	EICRA = 1 << ISC01 | 1 << ISC00;
	EIMSK = /*1 << INT0 |*/ 0x40;
	PCMSK2 = 0xFF;

	LCD_Init();
		
	
	//warning: TODO: add watchdog
	/*
	WDTCR = (1 << WDCE); // Watchdog change enable
	WDTCR = (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);  // Watchdog enable, 2048k cykles
	*/
	
	sei();
}




