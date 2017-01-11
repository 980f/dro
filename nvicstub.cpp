#include "wtf.h"
extern "C" {
//in case we aren't ready to bring the nvic into the build:
[[noreturn]] void generateHardReset(void){
  while(1){
    wtf(100002);
  }
}
}
