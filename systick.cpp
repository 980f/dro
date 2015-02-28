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
  unsigned int useCoreClock : 1; // 1: main clock, 0: vendor specific, via clockRate(-1) (stm32 fixes that at core/8)
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

  bool start(u32 reloader,bool useCore){
    enableInterrupt = 0;
    enableCounting = 0;
    useCoreClock = useCore;
    reload = reloader - 1; // for more precise periodicity
    volatile bool hack = rolledOver; // reading clears it
    enableCounting = 1;
    enableInterrupt = 1;
    return hack; // doubly ensure optimizer doesn't eliminate read of rolledOver
  } /* start */

  u32 ticksPerSecond(void){
    u32 effectiveDivider = reload + 1;
    return rate(clockRate(useCoreClock?0:-1), effectiveDivider);
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
  u32 reload=rate(clockRate(0), persecond);//try to use core clock
  bool useCore=reload<(1<<23);
  if(!useCore){//then caller better have setup the custom systick source to make the rate below value.
    reload=rate(clockRate(-1), persecond);
  }
  //and if out of range here --- user will figure it out due to horribly wrong timing.
  theSysTicker->start(reload,useCore);
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
 * using 2 lists would make the isr faster, but all the restarts slower.
 */
PolledTimer *PolledTimer::active = nullptr;

void PolledTimer::onTick(void){
  for(PolledTimer *scan = active; scan != nullptr; scan = scan->next) {
    if(! scan->done && --scan->systicksRemaining == 0) {
      scan->onDone();
    }
  }
} /* onTick */


PolledTimer::PolledTimer(void):
  done(1),
  systicksRemaining(0),
  next(nullptr){
  // insert into list, presumes that timer service isn't started until all timer objects are constructed.
  if(active == 0) {
    active = this;
  } else {
    //we do not need to lock in any sense, no isr will manipulate the list of PolledTimers as we have not provided for deleting them and they don't make sense as local objects. Creating a new one in an isr seems unuseful, but if allowed then we need to add an atomic-set-if-zero function to our cortexM3 specific code.
    for(PolledTimer *scan = active; scan != nullptr; scan = scan->next) {//standard single-linked-list append-at-end
      if(scan->next == nullptr) {
        scan->next = this;
        break;
      }
    }
  }
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
  restart(ticksForSeconds(seconds));
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
