/* dro project
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

#include "p1343_board.h"

using namespace LPC;

/** construction of this has been emptied of significance, all members are templated. */
const P1343devkit board;

//the next line actually sets up the clocks before main and pretty much anything else gets called:
const ClockStarter startup InitStep(InitHardware/2) (true,0,1000);//external clock wasn't working properly, need a test to check before switching to it.

using namespace LPC;

#include "pinirq.h"

const PinIrq<board.button.pini> pbwup;

HandleInterrupt(33){//port 2.9
  board.toggleLed(7);
  pbwup.prepare(); 
}

const Irq<gpioBankInterrupt(2)> gp2irq;//interrupt 54

HandleInterrupt(54){//this gets the button irq via any port 2 bit changing.
  //this got erratic board.toggleLed(5);
  board.led6 = board.wakeup;
  board.wakeup.irqAcknowledge();
}

//p0-4,p05 for qei.
const InputPin<0,4> primePhase;
const InputPin<0,5> otherPhase;
const Irq<primePhase.pini> prime;
#define myIrq 4

//should go up and down depending upon the input signals;
static int axis(0);

HandleInterrupt( myIrq ) {
  //prime phase interrupt
  bool dirbit = otherPhase;
  if (dirbit) {
    --axis; //ignoring quarter phase for now
  } else {
    ++axis; //will be +/-4 here
  }
  //??lpc input clear
}

#include "fifo.h"
static FifoBuffer<33> outgoing;
static FifoBuffer<63> incoming;

#include "uart.h"
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


const Uart theUart(uartReceiver,uartSender);
// vector isn't declared here as we need the concrete class.
// if we use function pointers instead of extension then we can do it here.
ObjectInterrupt(theUart.isr(), uartIrq);


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
  prime.enable();
  
  pbwup.configure(false,true);

  board.wakeup.setIrqStyle(AnyEdge,true);
  gp2irq.prepare();
  //no longer doing this prophylactically in the loop as we now use atomics rather than interrupt gating to deal with concurrency.
  IRQEN=1;//master enable
  while (1) {
    MNE(WFE);//wait for event, expecting interrupts to also be events.
    
//worked, now will do using isr:    board.led1 = board.button;
    if(slowToggle.hasFired()){
      ++events;  
      board.toggleLed(0);
      if((outgoing='A'+(events&15))){
        theUart.beTransmitting();
      }
    }
  }
  return 0;
}

