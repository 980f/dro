#include "nvic.h"
#include "systick.h"
#include "minimath.h"

// vendore specific code must provide a clock source, architecture various greatly across vendors.
extern u32 clockRate(int which);

/*
 * removing expired timers from list in the isr was not threadsafe, and pretty much 100% of the time
 * the item was restored to the list within a tick so removing it wasn't much of an optimzation.
 */
struct SysTicker {
  unsigned int enableCounting : 1; // enable counting
  unsigned int enableInterrupt : 1; // enable interrupt
  unsigned int fullspeed : 1; // 1: main clock, 0: that divided by 8 (St's choice, ignore their naming)
  unsigned int : 16 - 3;
  volatile unsigned int rolledOver : 1; // indicates rollover, clears on read
  unsigned int : 32 - 17;

  u32 reload; // (only 24 bits are implemented) cycle = this+1.

  u32 value; //
  // following is info chip provides to user,
  unsigned int tenms : 24; // value to get 100Hz
  unsigned int : 30 - 24;
  unsigned int refIsApproximate : 1;
  unsigned int noref : 1; // 1= no ref clock

  bool start(u32 reloader){
    enableInterrupt = 0;
    enableCounting = 0;
    fullspeed = 1;

    reload = reloader - 1; // for more precise periodicity
    bool hack = rolledOver; // reading clears it
    enableCounting = 1;
    enableInterrupt = 1;

    return hack; // just to ensure optimizer doesn't eliminate read of rolledOver
  } /* start */

  u32 ticksPerSecond(void){
    u32 effectiveDivider = reload + 1;

    if(! fullspeed) {
      effectiveDivider *= 8;
    }
    return rate(clockRate(-1), effectiveDivider);
  }

  u32 ticksForMicros(u32 us){
    return (us * ticksPerSecond()) / 1000000;
  }

  u32 ticksForMillis(u32 ms){
    return (ms * ticksPerSecond()) / 1000;
  }

  u32 ticksForHertz(float hz){
    return ratio(ticksPerSecond(), hz);
  }
};

soliton(SysTicker, 0xE000E010);

/** start ticking at the given rate.*/
void startPeriodicTimer(u32 persecond){
  // todo:2 fullspeed is hardcoded to 1 downstream of here, need to take care of that.
  theSysTicker->fullspeed = 1;
  if(! theSysTicker->fullspeed) {
    persecond *= 8; // times 8 here instead of /8 in the rate computation.
  }
  theSysTicker->start(rate(clockRate(-1), persecond));
}

double PolledTimer::secondsForTicks(u32 ticks){
  return ratio(double(ticks), double(theSysTicker->ticksPerSecond()));
}

double PolledTimer::secondsForLongTime(u64 ticks){
  return ratio(double(ticks), double(theSysTicker->ticksPerSecond()));
}

u32 PolledTimer::ticksForSeconds(float sec){
  if(sec <= 0) {
    return 0;
  }
  return theSysTicker->ticksForMillis(u32(sec * 1000));
}

u32 PolledTimer::ticksForMillis(int ms){
  if(ms <= 0) {
    return 0;
  }
  return theSysTicker->ticksForMillis(ms);
}

u32 PolledTimer::ticksForMicros(int ms){
  if(ms <= 0) {
    return 0;
  }
  return theSysTicker->ticksForMicros(ms);
}

u32 PolledTimer::ticksForHertz(float hz){
  return theSysTicker->ticksForHertz(hz);
}

/**
 * an isr will determine that the given time has expired,
 * but the interested code will have to look at object to determine that the event occurred.
 * as of this note all timers are touched every cycle even if they are finished.
 * using 2 lists would make the isr faster, but all the restarts slower. that makes a lot of sense.
 */
PolledTimer *PolledTimer::active = 0;

void PolledTimer::onTick(void){
  for(PolledTimer *scan = active; scan != 0; scan = scan->next) {
    if(! scan->done && --scan->systicksRemaining == 0) {
      scan->onDone();
    }
  }
} /* onTick */


PolledTimer::PolledTimer(void){
  done = 1;
  systicksRemaining = 0;
  next = 0;
  // insert into list, presumes that timer service isn't started until all timer objects are constructed.
  if(active == 0) {
    active = this;
  } else {
    for(PolledTimer *scan = active; scan != 0; scan = scan->next) {
      if(scan->next == 0) {
        scan->next = this;
        break;
      }
    }
  }
  // unlock
}

/** typically this is overloaded if latency is important.*/
void PolledTimer::onDone(void){
  done = true;
}

void PolledTimer::restart(u32 value){
  systicksRemaining = value + 1; // to ensure minimum wait even if tick fires while we are in this code.
  if(value > 0) {
    done = 0; // this makes this retriggerable
  } else { // negative waits are instantly done.
    done = 1;
    // #shall we call onDone()? no- that needs to be called from an ISR
  }
} /* restart */

void PolledTimer::restart(float seconds){
  if(seconds <= 0) {
    return;
  }
  restart(u32(seconds));
}

u32 milliTime = 0; // storage for global tick time.
u32 macroTime = 0;   // extended range tick time

HandleFault(15){ // 15: system tick
  ++milliTime;
  if(milliTime == 0) {
    // we have rolled over and anything waiting on an particular value will have failed
    ++macroTime; // but rollover of this is not going to happen for decades.
  }
  PolledTimer::onTick();
}

/** time since last rollover, must look at clock configuration to know what the unit is. */
u32 snapTime(void){
  return theSysTicker->reload - theSysTicker->value; // 'tis a downcounter, want time since reload.
}

u32 snapTickTime(void){
  theSysTicker->enableCounting = 0; // can't use bitlock on a field in a struct :(
  u32 retval = ((milliTime + 1) * (theSysTicker->reload + 1)) - theSysTicker->value;
  theSysTicker->enableCounting = 1; // todo:3 add some to systick to compensate for the dead time of this routine.
  return retval;
}

u64 snapLongTime(void){ // this presumes  little endian 64 bit integer.
  theSysTicker->enableCounting = 0; // can't use bitlock on a field in a struct :(
  u64 retval = milliTime | (u64(macroTime) << 32); // need a hack to get compiler to be efficient here.
  theSysTicker->enableCounting = 1; // todo:3 add some to systick to compensate for the dead time of this routine.
  return retval;
}

// end of file
