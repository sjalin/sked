#ifndef COOLING_H
#define COOLING_H

#include "env.h"
#include "tT.h"
#include "global.h"

void start_peltier(core_t *self, void *none);
void stop_peltier(core_t *self, void *none);
void start_fans(core_t *self, void *none);
void stop_fans(core_t *self, void *none);
int get_peltier_status();
void set_peltier_status(int onOff);
int get_fan_status();
void set_fan_status(int onOff);


#endif //COOLING_H
