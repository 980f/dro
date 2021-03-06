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

FifoBuffer<2048> trace;
class BufferedUart : public UartHandler {
public:

FifoBuffer<33> outgoing;
FifoBuffer<63> incoming;

public:
  bool loopback=false;

  bool receive(int indata) override {
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
  int send() override {
    kit.led5.toggle();
    int datum=outgoing;
    if(datum>=0){
      trace=datum;
    }
    return datum; //negative for fifo empty
  }

  /** try to put whole message into the sendbuffer */
  bool tryStuff(const char *message, unsigned sizeofMessage) {
    bool didsomething = false;
    while (outgoing.free() > sizeofMessage) {
      outgoing.stuff(message, sizeofMessage);
      didsomething = true;
    }
    beTransmitting();
    stuffsome();//uart is flawed, can't test THRE without potentially losing an incoming error.
    return didsomething;
  }
  void testFifo(){
    for(int i=1;i<1000;++i){
      outgoing=u8(i);
      int val=outgoing;
      if(val!=u8(i)){
        wtf(21);
      }
    }
  }
};

BufferedUart theUart;

HandleInterrupt(uartIrq) {
  theUart.isr();
}

const char testMessage[] = "3.1415962\r\n";
//"Jello Whirled\r\n";
const unsigned testBlockSize=sizeof(testMessage);

int main(void) {

  theUart.testFifo();


  kit.led4 = 0; //for intensity reference.
  theUart.initializeInternals(); //todo: use InitPriority mechanism for this?

  theUart.setFraming(8,Uart::NoParity,1);
//  /*9603*/71,1,10
  unsigned actualBaud = theUart.setBaudPieces(/*9603*/71,10,1, kit.ExpectedClock); //externally generated
  theUart.setRxLevel(14);
  theUart.reception(true);
  //set in constructor of uart object  theUart.irq(true);//most likely this is superfluous

  theUart.setLoopback(0);//hardware loopback
  theUart.loopback=1; //soft loopback.
  // interrupt defaults to edge sensitive
  //later++  qeiPrimeIrq.enable();

  startPeriodicTimer(1000); //shoot for millisecond resolution
  ledToggle.retrigger();    //seconds
                            //set by powerup/reset  IRQEN=1;//should already be on ...
  while (true) {
    //the WFE below wakes up at least every millisecond due to the systick being programmed for 1kHz.
    MNE(WFE); //WFE is more inclusive than WFI, events don't call an isr but do wakeup the core.
    if (ledToggle) {
      kit.led0.toggle();
    }

//    if(theUart.incoming.available()>=testBlockSize){
//      for(unsigned count=0;count <testBlockSize;++count){
//        if(theUart.incoming != testMessage[count]){
//          wtf(20); //fired off falsely, outgoing was shifted one,
//        }
//      }
//    }

    if (theUart.tryStuff(testMessage, testBlockSize)) {
      kit.led3.toggle();
    }


    int error=theUart.outgoing.boundsError(1);
    if(error){
      //failures quit happening ...
      wtf(19);
    }
  }
  return 0;
}

void SystemInit(){
  GPIO::Init();
}
