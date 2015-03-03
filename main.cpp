/* dro project
 */
#define useSTM32 1
//having to use my stm32P103 dev kit as I can't get an SWD downloader any time soon.

#if useSTM32
#include "gpio.h"
#include "exti.h"

#else
using namespace LPC;
#endif

#include "gpio.h"
#include "nvic.h"

#include "core_cmInstr.h"  //wfe OR wfi


//For pins the stm32 lib is using const init'ed objects, LPC templated. It will take some work to reconcile how the two vendors like to describe their ports,
//however the objects have the same usage syntax so only declarations need to be conditional on vendor.
#if useSTM32
#include "p103_board.h"
P103_board board;

Pin primePin(PB,11);
const InputPin primePhase(primePin);
Pin otherPin(PB,12);
const InputPin otherPhase(otherPin);

#define myIrq 40

Irq &pushButton(Exti::enablePin(board.buttonPin,false,true));
//todo: figure out a way to get this declared without manual look up
#define pbIrq 6

void IrqName(pbIrq) (void){
  board.led=!board.led;
}

#else
//p0-4,p05 for qei.
InputPin<0,4> primePhase;
InputPin<0,5> otherPhase;


#define myIrq 4

#endif

//should go up and down depending upon the input signals;
int axis(0);

//define myIRQ as the encoder's interrupt
//prime phase interrupt
void IrqName(myIrq)(void) {
  bool dirbit = otherPhase;
  if (dirbit) {
    --axis; //ignoring quarter phase for now
  } else {
    ++axis; //will be +/-4 here
  }
}


int main(void) {
  //soft stuff
  int events=0;
  board.led=0;


//  Exti::enablePin(otherPin);
  Irq &prime(Exti::enablePin(primePin,true,true));


  prime.enable();
  pushButton.enable();
  while (1) {
    MNE(WFE);
    ++events;
    board.led=events&1;
  }
  return 0;
}
