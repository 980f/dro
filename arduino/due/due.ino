Print &dbg(Serial);

//use a macro to get variable name:
#define Show(arg) dbg.print("\n" #arg ":"); dbg.print(arg)

#include "pinclass.h"

/** attached to the high side of an LED */
//const OutputPin<LED_BUILTIN> lamp;//13, also a motor control line on motor shield
/** by putting a LOW in the items below, a true turns the lamp on */
const OutputPin<PIN_LED_RXL,LOW> RX;  //72,73
const OutputPin<PIN_LED_TXL,LOW> TX;

//olimex protoshield:
const OutputPin<3> red;
const OutputPin<4> green;
const InputPin<6, LOW> button1;
const InputPin<7, LOW> button2;

#include "motorshield1.h"

const InputPin<5, HIGH> phB;
const InputPin<4, HIGH> phA;
const InputPin<3, HIGH> Index;

#include "timerservice.h"

#include "retriggerablemonostable.h"

RetriggerableMonostable lamprey(red, Ticks(350));
RegisterTimer(lamprey);
void triggerPulse(){
  lamprey.trigger();
}

#include "interruptPin.h"
void b2irqhandler(){
  green = button2;
  RX = green;
  if(button2) {
    M2 = 1 * (1 - button1);//go forward unless other button is pressed in which case stop
  }
  dbg.print("G");
  dbg.print(millis());
}

const InterruptPin<b2irqhandler, button2.number, FALLING> b2irq;

void b1irqhandler(){
  red = button1;
  TX = red;
  if(button1) {
    M2 = -1 * (1 - button2);//see greenLight();
  }
  dbg.print("R");
}

const InterruptPin<b1irqhandler, button1.number, FALLING> b1irq;


/** quadrature table:
 *      ___     ___
 *  ___/   \___/
 *        ___     ___
 *  _____/   \___/
 *
 *  A:HL=++
 *  A:LH=++
 *  B:HH=++
 *  B:LL=++
 */
/** base class for sharing across templated counter assemblies */
class QuadratureCounter {
public://eventually protect
  int location = 0;
  int dirstep;
public:
  template <bool primary> void step(bool A,bool B){
    if(primary) {
      if(phB==phA) {
        --location;
      } else {
        ++location;
      }
    } else {
      if(phA==phB) {
        ++location;
      } else {
        --location;
      }
    }
  } /* step */

  /** when cruising we only look at one phase, and possible one edge.
   * the dirstep records the direction and if one edge then set to 4, if both edges set to 2.
*/
  void cruiseDirection(int dirstep){
    this->dirstep=dirstep;
  }
  /** call this with each primary edge when mid-rate cruising */
  void cruiseStep(){
    location+=dirstep;
  }

  /** when using a hardware counter we retain the cruiseDirection setting and report the merge of the last incremental update with the count value.*/
  int fly(unsigned count)const{
    return location +dirstep*count;
  }
  /** when a 16 bit hardware counter wraps we need to update our internal value (unless we keep a rollover count with the counter).
   * Also if we reset the hardware counter such as with an index mark, we need to accumulate the pre-cleared value.
   * Unless the index is an absolute clear. */
  void rollover(unsigned count){
    location +=dirstep*count;
  }

} pos;

#include "hook.h"

//need forward references to change functions, so use a Hook to indirect those.
HookOnce<bool /*otherPin*/> cruiseChange;

//change to cruise mode must be synched with isr, so set a bit and let slow isr do the switch:
void phAirqHandler(){
  pos.step<true>(phA,phB);
  if(phA){
    cruiseChange(phB);
  }
}

void phBirqHandler(){
  pos.step<false>(phA,phB);
  //we never change phase on B events, makes life much simpler and would provide no real benefit.
}

const InterruptPin<phAirqHandler, phA.number, CHANGE> phAirq;
const InterruptPin<phBirqHandler, phB.number, CHANGE> phBirq;

void phFastHandler(){
  pos.cruiseStep();
  cruiseChange(phB);
}

const InterruptPin<phFastHandler,phA.number,FALLING> phFast;

/** isr part of going to cruise mode */
void startCruise(bool phb){
  phBirq=0;
  pos.cruiseDirection(phb?+4:-4);
  phFast.attach();
}

/** isr part of leaving cruise mode */
void endCruise(bool /*phb*/){
  phAirq=1;
  phBirq=1;
}

void beginCruising(){
  cruiseChange=startCruise;
}
void endCruising(){
  cruiseChange=endCruise;
}

//example of acting on timing event within the timer isr:
class Flasher: public CyclicTimer {
  using CyclicTimer::CyclicTimer;
  void onDone(void) override {
    CyclicTimer::onDone();
    RX.toggle();
  }

} flashLamp(451,false);
RegisterTimer(flashLamp);



class SlowPWMdemo: public SoftPWM {
public:
  using SoftPWM::SoftPWM;
  //it is a bad idea to do this much in an isr ...
  virtual void onToggle(bool on){
    if(!on) {
      dbg.print("\nSoftPWM:");
    } else {
      dbg.print(" to ");
    }
    dbg.print(millis());
  }

} spwmdemo(250,750,false);

RegisterTimer(spwmdemo);

void setup(){
  SerialUSB.begin(230400);//'native' usb port
  Serial.begin(2625000);//an actual uart, on DUE DMA is used so we should be able to push the baud rate quite high without choking the processor with interrupts.
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  b2irq.attach(true);//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  b1irq.attach(true);
  M2 = 0;
  M1 = 0;
  phAirq.attach(false);
  phBirq.attach(false);
} /* setup */

int lastlocation = 0;
void loop(){
  __WFE();
  if(changed(lastlocation,pos.location)) {
    //initially this printed sequential values, but eventually only every 6 or 7, then choked.Will try usbserial again, maybe it can keep up.
    dbg.print('\n');
    dbg.print(pos.location);
  }
}
