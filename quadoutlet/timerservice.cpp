#include "polledtimer.h"


static PolledTimer * table[10];//todo: compile time constant with default
static const unsigned TimerServiceNumTimers=sizeof(table)/sizeof(table[0]);

void LinkTimer(PolledTimer *timer){
  for(unsigned ti=TimerServiceNumTimers;ti-->0;){
    if(table[ti]==nullptr){
      table[ti]=timer;
      break;
    }
  }
}

void PolledTimerServer(void){
  for(unsigned ti=TimerServiceNumTimers;ti-->0;){
    if(table[ti]){
      table[ti]->check();      
    } else {
      break;
    }
  }
}

