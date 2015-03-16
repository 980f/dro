/* dro project
 */
//having to use my stm32P103 dev kit as I can't get an SWD downloader any time soon.

#include "wtf.h" //breakpoint for unhandled conditions

#include "gpio.h"
#include "nvic.h"
#include "clocks.h"

#include "systick.h"
using namespace SystemTimer;

#include "polledtimer.h"

#include "core_cmInstr.h"  //wfe OR wfi
#include "cruntime.h"
ClockStarter startup InitStep(InitHardware/2) (false,0,1000);

//For pins the stm32 lib is using const init'ed objects, LPC templated. It will take some work to reconcile how the two vendors like to describe their ports,
//however the objects have the same usage syntax so only declarations need to be conditional on vendor.
#if useSTM32
#include "exti.h"  //interrupts only tangentially coupled to i/o pins.
#include "p103_board.h"
P103_board board;

Irq &pushButton(Exti::enablePin(board.buttonPin,false,true));

void IrqName(6) (void){
  board.toggleLed();
  Exti::clearPending(board.buttonPin);
}

Pin primePin(PB,11);
const InputPin primePhase(primePin);
Pin otherPin(PB,12);
const InputPin otherPhase(otherPin);

Irq &prime(Exti::enablePin(primePin,true,true));

#define myIrq 40

#else

#include "p1343_board.h"
//InitStep(InitApplication)
P1343devkit board;//construction of this turns on internal peripherals and configures pins.


Irq pushButton(board.button.pini);

using namespace LPC;

//p0-4,p05 for qei.
InputPin<0,4> primePhase;
InputPin<0,5> otherPhase;


Irq prime(primePhase.pini);

#define myIrq 4

#endif

//should go up and down depending upon the input signals;
int axis(0);

HandleInterrupt( myIrq ) {
  //prime phase interrupt
  //void QEI (void) {
  bool dirbit = otherPhase;
  if (dirbit) {
    --axis; //ignoring quarter phase for now
  } else {
    ++axis; //will be +/-4 here
  }
#if useSTM32
  Exti::clearPending(primePin);
#else
  //??lpc input clear
#endif
}

#include "fifo.h"

u8 outbuf[33];
Fifo outgoing(sizeof(outbuf),outbuf);

u8 inbuf[33];
Fifo incoming(sizeof(inbuf),inbuf);

#include "uart.h"
/** called by isr on an input event.
 * negative values of @param are notifications of line errors, -1 for interrupts disabled */
bool uartReceiver(int indata){
  if(incoming.insert(indata)){
    return true;
  } else {
    wtf(24);
    return false;
  }
}

/** called by isr when transmission becomes possible.
 *  @return either an 8 bit unsigned character, or -1 to disable transmission events*/
int uartSender(){
  int outdata=outgoing.remove();
  return outdata;//negative for fifo empty
}

void prepUart(){
  theUart.setFraming("N81");
  theUart.setBaud(115200);
  theUart.setTransmitter(&uartSender);
  theUart.setReceiver(&uartReceiver);

  theUart.reception(true);
}

int main(void) {
  prepUart();

  CyclicTimer slowToggle; //since action is polled might as well wait until main to declare the object.
  slowToggle.restart(ticksForSeconds(3));


  //soft stuff
  int events=0;

  //  Exti::enablePin(otherPin);

  prime.enable();
  pushButton.enable();

  EnableInterrupts;

  while (1) {
    stackFault();
    MNE(WFI);
    ++events;
    board.led1=board.button;
    if(slowToggle.hasFired()){
      board.toggleLed();
      if(outgoing.insert('A'+(events&15))){
        theUart.beTransmitting();
      }
    }
  }
  return 0;
}
