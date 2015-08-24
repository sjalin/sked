#include <avr/interrupt.h>
#include <avr/io.h>
#include <math.h>
#include "debug.h"
#include "lcd.h"
#include "sched.h"
#include "stuff.h"
#include "temp.h"

float temp = 0; 
uint16 measuringTemp = 0; 
uint32 startTime = 0; 
uint32 changeTime = 0;
uint32 stopTime = 0; 
uint32 timer0Overflow = 0; 

void TempPowerOn();
void TempPowerOff();
void CalculateTemp(uint16 dummy);
//TODO: add functionaity to measure more than one temperature

ISR(TIMER2_OVF_vect){
	timer0Overflow++;
}


ISR(PCINT0_vect)
{
	if(1 == measuringTemp){
		if (PINE && 0x40)
			startTime = TCNT2;
			timer0Overflow = 0; 
			measuringTemp = 2;
	}else if (2 == measuringTemp){
		changeTime = TCNT2 + 256*timer0Overflow;
		measuringTemp = 3;
	}else if (3 == measuringTemp){
		stopTime = TCNT2 + 256*timer0Overflow;
		//TIMSK0 = 0;
		AddEvent(&CalculateTemp, (uint16) LOW, NOW, ONCE, DUMMY); 	
		EIMSK &= ~(1 << PCIE0); // Stop interrupt
		//TempPowerOff();
	}else{
		//TIMSK0 = 0;
		startTime = 0; 
		changeTime = 0; 
		stopTime = 0; 
		measuringTemp = 0;
		timer0Overflow = 0; 
		EIMSK &= ~(1 << PCIE0); // Stop interrupt
		//TempPowerOff();
	}

}



void InitTemp()
{
	DDRE &= ~0x60;
	//PORTE &= ~0x60; #warning: how to set the pullupp/down resistors
	EICRA = 1 << ISC00; // Interrupt on logical change
	EIMSK |= 1 << INT0; // External interrupt flag 0 enabled (not the specific pin yet           PINE 3 12       | 1 << PCIE0;
	PCMSK0 = 1 << PCINT6;//  | 1 << PCINT5;	

	//Timer 2 (8bit)
//	TCCR2A =  1 << CS20; //| 1 << CS01;
//	TIMSK2 = 1 << TOIE2;


	TempPowerOn();
}

void CalculateTemp(uint16 dummy){
	float DC; 
/*	if (startTime > stopTime){
		if (startTime > changeTime){
			changeTime += 65535 - startTime; 
		}else{
			changeTime = changeTime - startTime; 
		}
		
		stopTime += 65535 - startTime;
	}else{
		changeTime = changeTime - startTime; 
		stopTime = stopTime - startTime; 
	}*/
	changeTime -= startTime; 
	stopTime -= startTime; 	

	//Calculate temp
	DC = ((float)changeTime)/((float)stopTime);
	temp = (DC - 0.32)/0.0047;
	//temp = (((float)changeTime)/((float)stopTime) - 0.32)/(0.0047);
	
	startTime = 0; 
	changeTime = 0; 
	stopTime = 0; 
	measuringTemp = 0;
	timer0Overflow = 0; 
}

float GetTemp()
{
	//return 25.65;
	return temp; 
}

void StartTempMeasurement(uint16 dummy){
	EIMSK |= 1 << PCIE0; // Start interrupt
}

void UpdateTemp(uint16 dummy)
{
	/*
	if (0 == measuringTemp){
		measuringTemp = 1; 	
		//TempPowerOn();
		AddEvent(&StartTempMeasurement, LOW, MSEC(50), ONCE, DUMMY);
		//Set start of measurement in 50ms
	}
	*/
	float DC;
	uint32 first = 0; 
	uint32 second = 0; 
	
	BlinkLed(0x01); 
	while (PINE & 0x40){
		BlinkLed(0x10); 
	}
	BlinkLed(0x02); 
	while (PINE & 0x40 == 0){
		first++;
	}
	BlinkLed(0x03); 
	while (PINE & 0x40) {
		second++;
	}
	BlinkLed(0x04); 
	second += first; 
	
	DC = ((float)first)/((float)second);
	temp = (DC - 0.32)/0.0047;
	


}


void TempPowerOn(void)
{
	DDRA = 0xFF;
	PORTA = 0xFF;
}

void TempPowerOff(void)
{
	PORTA  &= ~(1<<PINA2);
}

