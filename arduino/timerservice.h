#ifndef TIMERSERVICE_H
#define TIMERSERVICE_H


#include "polledtimer.h"
extern "C" int sysTickHook(){
  PolledTimerServer();//all of our millisecond stuff hangs off of here.
  return false; //allowed standard code to run
}
#include "tableofpointers.h"

#endif
