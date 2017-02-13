Print &dbg(SerialUSB);

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

#include "motorshield1.h"  //two motors, M1 and M2. 

#include "quadrater.h" // quadrature tracker.
Quadrater<4,5,3> tracker;

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
    //analogWrite(10,100);
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
    //analogWrite(10,200);
    M2 = -1 * (1 - button2);//see greenLight();
  }
  dbg.print("R");
}

const InterruptPin<b1irqhandler, button1.number, FALLING> b1irq;

//example of acting on timing event within the timer isr:
class Flasher: public CyclicTimer {
  using CyclicTimer::CyclicTimer;
  void onDone(void) override {
    CyclicTimer::onDone();
    RX.toggle();
  }

} flashLamp(451,false);
RegisterTimer(flashLamp);

#include "softpwm.h"
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
  Serial.begin(57600);//an actual uart, on DUE DMA is used so we should be able to push the baud rate quite high without choking the processor with interrupts.
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  b2irq.attach(true);//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  b1irq.attach(true);//if we can establish that interrupts aren't globally on until after setup then we shall move attach() invocations to constructors.
  M2 = 0; //this should be moved into constructor
  M1 = 0;
  tracker.connect();
} /* setup */

int lastlocation = 0;
void loop(){
  __WFE();
  if(changed(lastlocation,tracker.position())) {
    //initially this printed sequential values, but eventually only every 6 or 7, then choked.Will try usbserial again, maybe it can keep up.
    dbg.print('\n');
    dbg.print(lastlocation);
  }
  if(lastlocation>500){
    tracker.beginCruising();
  }
  if(Serial){
    char c=Serial.read();
    Serial.print(c);
    switch(c){
      case '<': M2=1; break;
      case '>': M2=-1; break;
      case 27: M2=0; break;
      case '?': Serial.println(tracker.position()); break;
    }
  }
}
