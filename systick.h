#pragma once

#include "eztypes.h"

/**
system timer service (a not so fast timer)

Somewhere you must declare:
void SystemTickServer();

which will get called FROM AN ISR periodically.

*/
namespace SystemTimer {
/** start ticking at the given rate.
  * presumes system clock is already programmed and that it is the clock source*/
void startPeriodicTimer(u32 persecond);

/** time since last rollover (<1 ms if 1kHz, even shorter at higher sysfreq), only suitable for spinning in place for a short time.
NB: this is NOT in ticks, it is in probably 8MHz increments, but that may differ for some clock configurations.*/
u32 snapTime(void);
/** much longer time range but an expensive call, in units of system tick, call secondsForTicks()*/
u32 snapTickTime(void);
/** much much longer time range, with a range greater than the life of the instrument., call secondsForLongTime()*/
u64 snapLongTime(void);

double secondsForTicks(u32 ticks);
double secondsForLongTime(u64 ticks);

/** ticks necessary to get a delay of @param sec seconds*/
u32 ticksForSeconds(float sec);
u32 ticksForMillis(int ms);
u32 ticksForMicros(int us);
/** ticks necessary to get a periodic interrupt at the @param hz rate*/
u32 ticksForHertz(float hz); //approximate since we know a divide is required.
}
//functions to call on the SystemTick
typedef void (*SystemTicker)();
