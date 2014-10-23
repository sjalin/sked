#ifndef STUFF_H
#define STUFF_h

#include "system.h"

#define CLOCK_SOURCE_SPEED 	20000000
#define CLOCK_DEVIDER		1024	
#define TICKS_EVERY_MS 		CLOCK_SOURCE_SPEED/CLOCK_DEVIDER/1000


uint32 GetCurrentTime(); // returns time in ms
void SetNextWake(uint32 time);
#endif // STUFF_H
