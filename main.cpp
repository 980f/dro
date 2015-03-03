/* dro project
 */
#define useSTM32 1


#if useSTM32
#include "stm32.h"
#else
using namespace LPC;
#endif

#include "gpio.h" //alh
#include "nvic.h" //alh

#include "core_cmInstr.h"


//For pins the stm32 lib is using const init'ed objects, LPC templated. It will take some work to reconcile how the two vendors like to describe their ports,
//however the objects have the same usage syntax so only declarations need to be conditional on vendor.
#if useSTM32
#include "p103_board.h"
Pin primePin(PB,11);
const InputPin primePhase(primePin);
const InputPin otherPhase(Pin(PB,12));

#else
//p0-4,p05 for qei.
InputPin<0,4> primePhase;
InputPin<0,5> otherPhase;

#endif


#define myIrq 4

//Irq's cannot be const due to having a nesting control element within. If you don't want the nesting enable then you can use the baser class.
Irq prime(myIrq);

//should go up and down depending upon teh input signals;
int axis(0);

int initme(78);//testing linker
const int constly(95);


//prime phase interrupt
void IrqName(myIrq)(void) {
  bool dirbit = otherPhase;
  if (dirbit) {
    --axis; //ignoring quarter phase for now
  } else {
    ++axis; //will be +/-4 here
  }
}

P103_board board;

int main(void) {
  //soft stuff
  int events=initme;
  //hw stuff
  //GPIOInit();
  //interrupt defaults to edge sensitive
  prime.enable();

  while (1) {
    MNE(WFE);
    ++events;
  }
  return 0;
}
