#include "polledtimer.h"


/**
  */

#ifdef DUEDUINO  //have tablebuiler

#include "tableofpointers.h"
MakeTable(PolledTimer);

/** name required by systick.h. someday we'll get alias for static class member worked out */
void PolledTimerServer(void) {
  ForTable(PolledTimer){
    (*it)->check();
  }
} /* onTick */
#endif

#pragma GCC diagnostic warning "-Wunused-variable"

void PolledTimer::check(){
  if(running){
    if(--systicksRemaining==0){
      onDone();
    }
  }
}

PolledTimer::PolledTimer(void){
  running = 0;
  systicksRemaining = 0;
}

/** typically this is overloaded if latency is important.*/
void PolledTimer::onDone(void){
  running = 0;
}

void PolledTimer::restart(u32 value){
  systicksRemaining = value + 1; //to ensure minimum wait even if tick fires while we are in this code.
  if(value > 0) {//# leave expanded for debug
    running = 1; //this makes this retriggerable
  } else { //negative waits are instantly done.
    running = 0;
    //this is a failure to start so we don't call the onDone..
  }
} /* restart */

void PolledTimer::restart(float seconds){
  if(seconds<=0){
    return;
  }
  restart(u32(seconds));
}

void PolledTimer::freeze(){
  running = 0;//precludes isr touching remaining time, and onDone doesn't get called.
}

////////////////////////////

bool CyclicTimer::hasFired(){
  //maydo: LOCK, present intended uses can miss a tick without harm so we don't bother.
  ClearOnExit <u32> z(fired); //trivial usage of ClearOnExit, for testing that.
  return fired != 0;
}

void CyclicTimer::onDone(){
  ++fired;
  //pointless: PolledTimer::onDone();
  retrigger();
}
