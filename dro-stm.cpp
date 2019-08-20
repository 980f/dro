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


static Pin buttonPin(PA,9);

static const Irq &pushButton(Exti::enablePin(buttonPin,false,true));

void IrqName(6) (void){
  board.toggleLed();
  Exti::clearPending(buttonPin);
}

static Pin primePin(PB,1);
const InputPin primePhase(primePin);
static Pin otherPin(PB,2);
const InputPin otherPhase(otherPin);

static const Irq &prime(Exti::enablePin(primePin,true,true));

#define myIrq 40

//should go up and down depending upon the input signals;
static int axis(0);

HandleInterrupt( myIrq ) {
  //prime phase interrupt
  //void QEI (void) {
  bool dirbit = otherPhase;
  if (dirbit) {
    --axis; //ignoring quarter phase for now
  } else {
    ++axis; //will be +/-4 here
  }
  Exti::clearPending(primePin);
}

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

//LPC
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

//LPC specific:
//#include "packdatatest.h"
//static packdatatest ender  __attribute((section(".rodata.packdata.last"))) ={'\0'};


int main(void) {
//  prepUart();
  CyclicTimer slowToggle(ticksForSeconds(3)); //since action is polled might as well wait until main to declare the object.
  //soft stuff
  int events=0;
  //  Exti::enablePin(otherPin);

  prime.enable();
  pushButton.enable();

  while (1) {
    stackFault();//useless here, present to test compilation.
    //re-enabling in the loop to preclude some handler shutting them down. That is unacceptable, although individual ones certainly can be masked.
    IrqEnable=1;//master enable
    MNE(WFI);//wait for interrupt. Can't get a straight answer from arm on whether WFE also triggers on interrupts.
    ++events;
    board.led=bool(buttonPin);
    if(slowToggle.hasFired()){
      board.toggleLed();
      if(outgoing.insert('A'+(events&15))){
//        theUart.beTransmitting();
      }
    }
  }
  return 0;
}
