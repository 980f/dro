/* dro project, this time with an arduino DUE
 */

#include "wtf.h" //breakpoint for unhandled conditions

#include "gpio.h"
#include "nvic.h"
#include "clocks.h"

#include "systick.h"
using namespace SystemTimer;
#include "polledtimer.h"
#include "tableofpointers.h"

#include "core_cmInstr.h"  //wfe OR wfi
#include "cruntime.h"

#include "arduinodue.h" //ArduinoDue

using namespace SAM;

//the next line actually sets up the clocks before main and pretty much anything else gets called:
ClockStarter startup InitStep(InitHardware/2) (true,0,1000);

ArduinoDue board InitStep(InitApplication);//construction of this turns on internal peripherals and configures pins.

//HandleInterrupt(54){//this gets the button irq
//  board.toggleLed(5);
//  board.button.irqAcknowledge();
//}

////p0-4,p05 for qei.
//InputPin<0,4> primePhase;
//InputPin<0,5> otherPhase;
//Irq prime(primePhase.pini);
//#define myIrq 4

//should go up and down depending upon the input signals;
static int axis(0);

//HandleInterrupt( myIrq ) {
//  //prime phase interrupt
//  bool dirbit = otherPhase;
//  if (dirbit) {
//    --axis; //ignoring quarter phase for now
//  } else {
//    ++axis; //will be +/-4 here
//  }
//  //??lpc input clear
//}

#include "fifo.h"
static FifoBuffer<33> outgoing;
static FifoBuffer<63> incoming;

/** called by isr on an input event.
 * negative values of @param are notifications of line errors, -1 for interrupts disabled */
bool uartReceiver(int indata){
  if((incoming=u8(indata))){
    return true;
  } else {
    wtf(24);
    return false;
  }
}

/** called by isr when transmission becomes possible.
 *  @returns either an 8 bit unsigned character, or -1 to disable transmission events*/
int uartSender(){
  return outgoing;//negative for fifo empty
}
#include "uart.h"
const SAM::Uart theUart(&uartReceiver,&uartSender);

void prepUart(){
  theUart.setFraming("N81");
  theUart.setBaud(115200);
  theUart.reception(true);
  theUart.irq(true);
}

#include "minimath.h"

static CyclicTimer slowToggle; //since action is polled might as well wait until main to declare the object.
RegisterTimer(slowToggle);

MakeRef(SystemTicker,PolledTimerServer);

int main(void) {
  prepUart();
  slowToggle.restart(ticksForSeconds(0.333));
  //soft stuff
  int events=0;
//  prime.enable();
//  pushButton.enable();//@nvic
//  board.button.setIrqStyle(IrqStyle::AnyEdge,true);
//  Irq gp2irq(gpioBankInterrupt(2));
//  gp2irq.prepare();
  //no longer doing this prophylactically in the loop as we now use atomics rather than interrupt gating to deal with concurrency.
  IRQEN=1;//master enable
  while (1) {
    MNE(WFE);//wait for event, expecting interrupts to also be events.
    ++events;
    if(slowToggle.hasFired()){
      board.LED.toggle();
      if((outgoing='A'+(events&15))){
        theUart.beTransmitting();
      }
    }
  }
  return 0;
}

