#ifndef _PLAYER_CLOCK_H_
#define _PLAYER_CLOCK_H_

#include <math.h>

#include "player_pod.h"

double get_clock(Clock *c);

void set_clock_at(Clock *c, double pts, int serial, double time);

void set_clock(Clock *c, double pts, int serial);

void set_clock_speed(Clock *c, double speed);

void init_clock(Clock *c, int *queue_serial);

void sync_clock_to_slave(Clock *c, Clock *slave);



#endif // _PLAYER_CLOCK_H_