#include <avr/interrupt.h>
#include <avr/io.h>

#include "lcd.h"
#include "sched.h"
#include "system.h"
#include "temp.h"
#include "debug.h"

//TODO: make all functions the same cameCase standard
//TODO: add leds to card to make debugging 


void PrintTemp(uint16 dummy)
{
	/*static float number = -13.98;
	
	number += 10.41;
	number *= -1;*/ 
	PrintFloat(GetTemp()); 
}

int main()
{
	InitSystem();
	InitLCD();
	InitTemp();
	InitScheduler();
	InitDebug();
	
	AddEvent(&UpdateTemp, (uint16) MEDIUM, SEC(1), CYCLIC, DUMMY);
	AddEvent(&PrintTemp, (uint16) MEDIUM, MSEC(500), CYCLIC, DUMMY); 

	
	//AddEvent(&print_second, (uint16) MEDIUM, SEC(1), CYCLIC, DUMMY);
//	AddEvent(&BlinkLed, (uint16) MEDIUM, MSEC(500), CYCLIC, 0x1);
//	AddEvent(&BlinkLed, (uint16) MEDIUM, SEC(5), CYCLIC, 0x2);
	
	StartScheduler();
	
	return 0; 
}


