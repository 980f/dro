/* dro project
 * this attempt is with lpc1343, using Olimex P1343 dev kit (8 LED's :)
 */
#include "quadraturecounter.h"
QuadratureCounter pos; //as in 'position'

#include "gpio.h"
#include "lpc13xx.h"
#include "nvic.h"

//shared ~millisecond services:
#include "msservice.h"

CyclicTimer ledToggle(750); //todo: clearer timebase
RegisterTimer(ledToggle);   //a polled timer, a macro for 'declare and register' could be made but it would have to be variadic to allow constructor args.

#include "core_cmInstr.h" //wfe OR wfi
#include "p1343_board.h"  //P1343devkit //for led's
#include "wtf.h"

using namespace LPC;
const P1343devkit kit;


// using p0-4,p05 for qei.
const InputPin<PortNumber(0), BitNumber(4)> primePhase;
const InputPin<0, 5> otherPhase;

const Irq<4> qeiPrimeIrq;
const Irq<5> qeiOtherIrq;

// prime phase interrupt
HandleInterrupt(4) { // interrupt number and bit number coincidentally the same.
  pos.step<true>(primePhase == otherPhase);
}

HandleInterrupt(5) { // interrupt number and bit number coincidentally the same.
  pos.step<false>(primePhase == otherPhase);
}

/////////////{
#include "fifo.h"

#include "uart.h"

//the mutable parts of BufferedUart, removed so that that class can be const.
FifoBuffer<33> outgoing;
FifoBuffer<63> incoming;

class BufferedUart : public UartHandler {
public:
public:
  bool receive(int indata) const override {
    if ((incoming = u8(indata))) {
      kit.led7.toggle();
      return true;
    } else {
      kit.led6.toggle();
      wtf(24);
      return false;
    }
  }
  /** called when transmission becomes possible.
       *  @return either an 8 bit unsigned character, or -1 to disable transmission events*/
  int send() const override {
    kit.led5.toggle();
    return outgoing; //negative for fifo empty
  }

  bool tryStuff(const char *message, unsigned sizeofMessage) const {
    bool didsomething = false;
    if (outgoing.free() > sizeofMessage) {
      outgoing.stuff(message, sizeofMessage);
      didsomething = true;
    }
    beTransmitting();
    stuffsome();
    return didsomething;
  }
};

const BufferedUart theUart;

HandleInterrupt(uartIrq) {
  theUart.isr();
}

const char testMessage[] = "Jello Whirled\r\n";

int main(void) {
  kit.led4 = 0; //for intensity reference.
  theUart.initializeInternals();

  theUart.setFraming("8N1");
  unsigned actualBaud = theUart.setBaudPieces(6, 12, 1, kit.ExpectedClock); //externally generated

  theUart.reception(true);
  //set in constructor of uart object  theUart.irq(true);//most likely this is superfluous

  theUart.setLoopback(0);
  // interrupt defaults to edge sensitive
  //later++  qeiPrimeIrq.enable();

  startPeriodicTimer(1000); //shoot for millisecond resolution
  ledToggle.retrigger();    //seconds
                            //set by powerup/reset  IRQEN=1;//should already be on ...
  while (1) {
    //the WFE below wakes up at least every millisecond due to the systick being programmed for 1kHz.
    MNE(WFE); //WFE is more inclusive than WFI, events don't call an isr but do wakeup the core.
    if (ledToggle) {
      kit.led0.toggle();
    }
    if (theUart.tryStuff(testMessage, sizeof(testMessage))) {
      kit.led3.toggle();
    }
  }
  return 0;
}

void SystemInit(){
  GPIO::Init();
}
