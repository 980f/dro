#pragma ide diagnostic ignored "hicpp-signed-bitwise"
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
static ClockStarter startup InitStep(InitHardware/2) (true,0,1000);//external wasn't working properly, need a test to check before switching to it.

//For pins the stm32 lib is using const init'ed objects, LPC templated. It will take some work to reconcile how the two vendors like to describe their ports,
//however the objects have the same usage syntax so only declarations need to be conditional on vendor.
#include "exti.h"  //interrupts only tangentially coupled to i/o pins.
#include "bluepill.h" //declares a board.

#include "fifo.h"

static uint8_t outbuf[33];
static Fifo outgoing(sizeof(outbuf),outbuf);

static uint8_t inbuf[33];
static Fifo incoming(sizeof(inbuf),inbuf);

#include "uart.h"
/** called by isr on an input event.
 * negative values of @param are notifications of line errors, -1 for interrupts disabled */
bool uartReceiver(int indata){
  if(incoming.insert(char(indata))){
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

//void prepUart(){
//  theUart.setFraming("N81");
//  theUart.setBaud(115200);
//  theUart.setTransmitter(&uartSender);
//  theUart.setReceiver(&uartReceiver);
//
//  theUart.reception(true);
//  theUart.irq(true);
//}

#include "minimath.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main() {
//  prepUart();
  CyclicTimer slowToggle(ticksForSeconds(3)); //since action is polled might as well wait until main to declare the object.
  //soft stuff
  int events=0;

  while (true) {
    //re-enabling in the loop to preclude some handler shutting them down. That is unacceptable, although individual ones certainly can be masked.
    IrqEnable=true;//master enable
    MNE(WFE);//wait for interrupt. Can't get a straight answer from arm on whether WFE also triggers on interrupts.
    ++events;
    if(slowToggle.hasFired()){
      board.toggleLed();
      if(outgoing.insert('A'+(events&15))){
//        theUart.beTransmitting();
      }
    }
  }
}
