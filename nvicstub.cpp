#include "wtf.h"
//in case we aren't ready to bring the nvic.cpp into the build:

extern "C" void generateHardReset(void){
  while(1){
    wtf(100002);
  }
}

