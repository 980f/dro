#pragma ide diagnostic ignored "hicpp-signed-bitwise"
/* dro project
 * These devices have hardware quadrature counting on many of their timers.
 * F103: Tim1,8,2,3,4,5 so if used in pairs that is 3 axes.
 * Bluepill only has 4 of those, so 2 axes if we use the "both channels" approach of dealing with jitter.
 * If we aren't going to use both channels then we have to swap which edges are counted.
 *
 * SMS field:
 001: Encoder mode 1 - Counter counts up/down on TI2FP1 edge depending on TI1FP2 level.
 010: Encoder mode 2 - Counter counts up/down on TI1FP2 edge depending on TI2FP1 level.
 011: Encoder mode 3 - Counter counts up/down on both TI1FP1 and TI2FP2 edges depending on the level of the other input.
 So,
 */

#include <quadraturecounter.h>

#include "wtf.h" //breakpoint for unhandled conditions

#include "gpio.h"
#include "nvic.h"
#include "clocks.h"

#include "systick.h"

using namespace SystemTimer;

#include "sharedtimer.h"

#include "core_cmInstr.h"  //wfe OR wfi
#include "cruntime.h"
#include "stackfault.h"

static ClockStarter startup InitStep(InitHardware / 2)(true, 0, 1000); //external wasn't working properly, need a test to check before switching to it.

//For pins the stm32 lib is using const init'ed objects, LPC templated. It will take some work to reconcile how the two vendors like to describe their ports,
//however the objects have the same usage syntax so only declarations need to be conditional on vendor.
#include "exti.h"  //interrupts only tangentially coupled to i/o pins.

Pin primePin(PB, 11);
const InputPin primePhase( {PB,11});//primePin);
Pin otherPin(PB, 12);
const InputPin otherPhase( {PB,12});//&otherPin);

const Irq &prime(Exti::enablePin(primePin, true, true));
#define myIrq 40

#include "uart.h"

#if useP103
#include "p103_board.h"
P103_board board;

Irq &pushButton(Exti::enablePin(board.buttonPin,false,true));

void IrqName(6) (){
  board.toggleLed();
  Exti::clearPending(board.buttonPin);
}

Uart theUart(2);
#elif DEVICE==103//presume bluepill for now, add other boards as the need arises
#include "bluepill.h"
Uart theUart(1);
#elif DEVICE==411
#include "blackpill.h"
Uart theUart(1);
#endif
///////////////////////////////////////////////////
/** we will have multiple instances of encoders being monitored */
class Axis {
public:
  /** for the UI */
  struct Description {
    const char *name;
    const char *unit;//as in inches, mm,mils
    const float resolution;//encoder tick in above unit
    Description(const char *name,const char *unit,const float tickSize):name(name),unit(unit),resolution(tickSize){}
  };
  Description description;
  /** internal position in encoder resolution steps */
  QuadratureCounter counter;

  /** when cruising we set stepsize to 2 */
  enum Stepby {
    BothPhases=1, //gets called
    BothEdges=2,
    OneEdge=4
  } stepsize;
  Axis(Description desc): description(desc) {}

};

Axis axes[] = {{{"X","mil",0.1}}};
const unsigned numAxes=countof(axes);

enum AxisMessage {
  not_homed, position, speed, configuration,numMessages
};
/** we score board messages and when uart is free we send one and clear its flag */
bool messages[numAxes][numMessages];
/** called by isr on an input event.
 * negative values of @param are notifications of line errors, -1 for interrupts disabled */
bool uartReceiver(int indata) {
  // if (incoming.insert(char(indata))) {
  //   return true;
  // } else {
  //   wtf(24);
     return false;
  // }
}

/** called by isr when transmission becomes possible.
 *  @return either an 8 bit unsigned character, or -1 to disable transmission events*/
int uartSender() {
  // int outdata = outgoing.remove();
  return 0;//outdata; //negative for fifo empty
}

void prepUart() {
  theUart.setParams(115200),
    // theUart.setTransmitter(&uartSender);
    // theUart.setReceiver(&uartReceiver);
    //
    // theUart.reception(true);
    theUart.irq.enable();
}

HandleInterrupt(myIrq) {
  bool dirbit = otherPhase;
  axes[0].counter.stepOne(dirbit);
  Exti::clearPending(primePin);
}
#include "minimath.h"

[[noreturn]] int main() {
  prepUart();
  CyclicTimer slowToggle(ticksForHertz(3)); //since action is polled might as well wait until main to declare the object.
  unsigned events = 0;
  prime.enable();
#ifdef useP103
  pushButton.enable();
#endif
  while (true) {
    stackFault(); //useless here, present to test compilation.
    //re-enabling in the loop to preclude some handler shutting them down. That is unacceptable, although individual ones certainly can be masked.
    IrqEnable = true; //master enable
    MNE(WFI); //wait for interrupt. Can't get a straight answer from arm on whether WFE also triggers on interrupts.
    ++events;

    if (slowToggle.hasFired()) {
#ifdef useP103
      board.toggleLed();  //replace with psuedo boolean and conditionally create it to either useless gpio or one that has an LED
#endif
      // if (outgoing.insert('A' + (events & 15))) {
      //   theUart.beTransmitting();
      // }
    }
  }
}

#pragma clang diagnostic pop
