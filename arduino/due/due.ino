#include <sam3xcounter.h>
#include <interruptGuard.h>
#include <ignoresignwarnings.h>
#include <interruptPin.h>
#include <pinclass.h>


#include "pinclass.h"
/** attached to the high side of an LED */
const OutputPin<LED_BUILTIN> lamp;
/** by putting a LOW in the items below, a true turns the lamp on */
const OutputPin<PIN_LED_RXL,LOW> RX;
const OutputPin<PIN_LED_TXL,LOW> TX;

//olimex protoshield:
const OutputPin<3> red;
const OutputPin<4> green;
const InputPin<6, LOW> button1;
const InputPin<7, LOW> button2;

#include "polledtimer.h"
extern "C" int sysTickHook(){
  PolledTimerServer();//all of our millisecond stuff hangs off of here.
  return false; //allowed standard code to run
}

//#include "retriggerablemonostable.h"
RetriggerableMonostable lamprey(red,350);
RegisterTimer(lamprey);

void triggerPulse(){
  lamprey.trigger();
}
const InterruptPin<triggerPulse,button1.number,FALLIN> redirq;

#define Show(arg) SerialUSB.print("\n" #arg ":");SerialUSB.print(arg)

#include "interruptPin.h"
void greenLight() {
  green = button2;
  RX=green;
}

void redLight(){
  red = button1;
  TX=red;
}

//const InterruptPin<redLight, button1.number, CHANGE> redirq;

const InterruptPin<greenLight, button2.number, CHANGE> greenirq;
  

class Flasher: public CyclicTimer {
  using CyclicTimer::CyclicTimer;
  void onDone(void) override {
    CyclicTimer::onDone();
    lamp.toggle();
  }
} flashLamp(451);

//#include "tableofpointers.h"
RegisterTimer(flashLamp);

CyclicTimer noncritical(1250);
RegisterTimer(noncritical);

CyclicTimer shorttimer(250);
RegisterTimer(shorttimer);


void setup() {
  SerialUSB.begin(230400);//'native' usb port
  Serial.begin(115200);//an actual uart
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  greenirq.attach(true);//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  redirq.attach(true);
}

char indicator='-';
void loop() {
  __WFE();//this made the beat between the two timers go away, which is a good thing.
  if(shorttimer.hasFired()){
    SerialUSB.print(indicator);
  }
  if(noncritical.hasFired()){
    SerialUSB.println();
  }
}
