#include "cooling.h"

//Globals
int peltierOn = 0; 
int fanOn = 0;


int init_cooling()
{
	DDRB = 0x03;
	return 0;
}


void start_peltier(core_t *self, void *none)
{
	PORTB |= (1<<PIN7);
	set_peltier_status(1);
}

void stop_peltier(core_t *self, void *none)
{
	PORTB &= !(1<<PIN7);
	set_peltier_status(0);
}

void start_fans(core_t *self, void *none)
{
	PORTB |= (1<<PIN6);
	set_fan_status(1);
}

void stop_fans(core_t *self, void *none)
{
	PORTB &= !(1<<PIN6);
	set_fan_status(0);
}


int get_peltier_status()
{
	return peltierOn;
}

void set_peltier_status(int onOff)
{
	peltierOn = onOff;
}

int get_fan_status()
{
	return fanOn;
}

void set_fan_status(int onOff)
{
	fanOn = onOff;
}
