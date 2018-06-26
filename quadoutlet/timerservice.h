#ifndef TIMERSERVICE_H
#define TIMERSERVICE_H
#include "polledtimer.h"

#ifdef DUEDUINO

extern "C" int sysTickHook(){
  PolledTimerServer();//all of our millisecond stuff hangs off of here.
  return false; //allowed standard code to run
}
#include "tableofpointers.h"

/** a server that will update all registered timers.
 * You must arrange for it to get called with each tick */

//registration is compile time, your object must be static to be party to this service.
#define RegisterTimer(name) Register(PolledTimer,name)


#else
//polled one.

extern void LinkTimer(PolledTimer *timer);

struct TimerHiker {
  TimerHiker(PolledTimer *timer){
    LinkTimer(timer);
  }
};

#define RegisterTimer(name) TimerHiker hikeTimer##name(& name)

#endif
extern void PolledTimerServer(void);

void pollTicker();

#endif
