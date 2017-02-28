/* dro project
 * this attempt is with lpc1343, using Olimex P1343 dev kit (8 LED's :)
 */
#include "quadraturecounter.h"
QuadratureCounter pos;//as in 'position'

#include "lpc13xx.h"
#include "gpio.h" 
#include "nvic.h" 

//shared ~millisecond services:
#include "systick.h"
using namespace SystemTimer; 
#include "polledtimer.h"
#include "tableofpointers.h"
MakeRef(SystemTicker,PolledTimerServer); //put the polledtimer poller in the systick isr handler list

CyclicTimer ledToggle(750);//todo: clearer timebase
RegisterTimer(ledToggle); //a polled timer, a macro for 'declare and reigster' could be made but it would have to be variadic to allow constructor args.

#include "core_cmInstr.h"  //wfe OR wfi
#include "cruntime.h" 

#include "p1343_board.h" //P1343devkit //for led's
#include "wtf.h" 
using namespace LPC;

//todo: should inherit from a board description header
const unsigned ExpectedClock=12000000;

// using p0-4,p05 for qei.
const InputPin<PortNumber(0), BitNumber(4)> primePhase(BusLatch);
const InputPin<0, 5> otherPhase(BusLatch);

Irq<4> qeiPrimeIrq;
Irq<5> qeiOtherIrq;

// prime phase interrupt
HandleInterrupt(4){ // interrupt number and bit number coincidentally the same. 
  pos.step<true>(primePhase==otherPhase);
}

HandleInterrupt(5){ // interrupt number and bit number coincidentally the same. 
  pos.step<false>(primePhase==otherPhase);
}

/////////////{
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

const char testMessage[]="Jello Whirled\r\n";

#include "uart.h"
Uart theUart(&uartReceiver,&uartSender);

HandleInterrupt(uartIrq){
  theUart.isr();
}

P1343devkit kit;
//CyclicTimer ledToggle;

int main(void){
  theUart.setFraming("8N1");
  unsigned actualBaud=theUart.setBaudPieces(6,12,1,ExpectedClock);//externally generated 
  
  theUart.reception(true);
  theUart.irq(true);//most likely this is superfluous

  // interrupt defaults to edge sensitive
  qeiPrimeIrq.enable();

  startPeriodicTimer(1000);//shoot for millisecond resolution
  ledToggle.retrigger();//seconds
  while(1) {
    MNE(WFE);//WFE is more inclusive than WFI, events don't call an isr but do wakeup the core.
    if(ledToggle){
      kit.led3=!kit.led3;
    }
    if(outgoing.free()>sizeof(testMessage)){
      outgoing.stuff(testMessage,sizeof(testMessage));
      theUart.beTransmitting();
    }
  }
  return 0;//avert useless warning
} // main

