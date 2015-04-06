/* dro project
 */

#include "wtf.h" //breakpoint for unhandled conditions

#include "gpio.h"
#include "nvic.h"
#include "clocks.h"

#include "systick.h"
using namespace SystemTimer;
#include "polledtimer.h"

#include "core_cmInstr.h"  //wfe OR wfi
#include "cruntime.h"
//the next line actually sets up the clocks before main and pretty much anything else gets called:
ClockStarter startup InitStep(InitHardware/2) (true,0,1000);//external wasn't working properly, need a test to check before switching to it.

#include "p1343_board.h"
InitStep(InitApplication)
P1343devkit board;//construction of this turns on internal peripherals and configures pins.
Irq pushButton(board.button.pini);

using namespace LPC;

HandleInterrupt(P1343ButtonIrqNum){
  board.toggleLed(4);
  board.but1.irqAcknowledge();
}

HandleInterrupt(54){
  board.toggleLed(5);
  board.but1.irqAcknowledge();
}

//p0-4,p05 for qei.
InputPin<0,4> primePhase;
InputPin<0,5> otherPhase;
Irq prime(primePhase.pini);
#define myIrq 4

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
  //??lpc input clear
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
  theUart.irq(true);
}
#include "minimath.h"

int main(void) {
  prepUart();
  CyclicTimer slowToggle; //since action is polled might as well wait until main to declare the object.
  slowToggle.restart(ticksForSeconds(1.333));
  //soft stuff
  int events=0;

  prime.enable();
  pushButton.enable();//@nvic
  board.but1.setIrqStyle(GPIO::LowEdge,true);
  Irq gp2irq(gpioBankInterrupt(2));
  gp2irq.prepare();

  while (1) {
    stackFault();//useless here, present to test compilation.
    //re-enabling in the loop to preclude some handler shutting them down. That is unacceptable, although individual ones certainly can be masked.
    EnableInterrupts;//master enable
    MNE(WFI);//wait for interrupt. Can't get a straight answer from arm on whether WFE also triggers on interrupts.
    ++events;
    board.led1=board.button;
    if(slowToggle.hasFired()){
      board.toggleLed(0);
      if(outgoing.insert('A'+(events&15))){
        theUart.beTransmitting();
      }
    }
  }
  return 0;
}
