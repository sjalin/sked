#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "system.h"

#define SEC(x)	x*1000
#define MSEC(x) x 

#define NUMBER_OF_PRIOS (BACKGROUND +1)

typedef enum{
   URGENT = 0,
   HIGH,
   MEDIUM,
   LOW,
   BACKGROUND
}prio_t;

enum{ //Scheduling function returns (errors)
   INVALID_PRIO = -2,
   LIST_FULL = -1,
   OK = 0
};

typedef enum{
	ONCE = 0,
	CYCLIC
}cyclic_control_t;

void InitScheduler();
int16 AddEvent( void (*functionPtr) (uint16), prio_t prio, uint32 after,  cyclic_control_t cyclic, uint16 arg);
void StartScheduler();
void StopScheduler();
//void RemoveEvent( void (*functionPtr) (int), int event);
uint32 GetTimeOfNextEvent();


#endif //SCHEDULER_H
