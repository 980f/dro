/* dro project
 */
#define useSTM32 1


#include "core_cmInstr.h"
#if useSTM32
#niclude "
#else
#include "gpio.h" //alh
#endif

#include "nvic.h" //alh

using namespace LPC;

//p0-4,p05 for qei.
InputPin<0,4> primePhase;
InputPin<0,5> otherPhase;

//Irq's cannot be const due to having a nesting control element within.
Irq prime(4);

//#define primePhase 4
//#define otherPhase 5
//#define togglePhase(ph) (primePhase+otherPhase-(ph))

int axis(0);
int initme(78);
//prime phase interrupt
void IrqName(4)(void) {
  bool dirbit = otherPhase;
  if (dirbit) {
    --axis; //ignoring quarter phase for now
  } else {
    ++axis; //will be +/-4 here
  }
}


int wtf(int complaint) {
  static int lastWtf = 0;
  static int repeats = 0;
  if (complaint) {
    if (changed(lastWtf, complaint)) {
      repeats = 0;
      return complaint;
    } else {
      ++repeats;
    }
  }
  return 0;
}

int main(void) {
  //soft stuff
  int events=initme;
  //hw stuff
  //GPIOInit();
  //interrupt defaults to edge sensitive
  prime.enable();

  while (1) {
    //__WFE();
    MNE(WFE);
    ++events;
  }
  return 0;
}
