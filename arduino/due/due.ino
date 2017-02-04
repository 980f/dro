

#include "pinclass.h"
/** attached to the high side of an LED */
const OutputPin<LED_BUILTIN> lamp;

//olimex protoshield:
const OutputPin<3> red;
const OutputPin<4> green;
const InputPin<6, LOW> button1;
const InputPin<7, LOW> button2;


#define Show(arg) SerialUSB.print("\n" #arg ":");SerialUSB.print(arg)
void dump() {
 
}

#include "interruptPin.h"
void greenLight() {
  green = button2;
}

void redLight(){
  red = button1;
}

const InterruptPin<redLight, button1.number, CHANGE> redirq;

const InterruptPin<greenLight, button2.number, CHANGE> greenirq;

//extended range of milliseconds.
static unsigned rollovers=0;
void rollover(){
  if(millis()==~0){
    ++rollovers;
  }
}
  
#include "polledtimer.h"

class Flasher: public CyclicTimer {
  void onDone(void) override {
    CyclicTimer::onDone();
    lamp.toggle();
  }
} flashLamp;

#include "tableofpointers.h"
RegisterTimer(flashLamp);


extern "C" int sysTickHook(){
  PolledTimerServer();
  rollover();
  return false; //allowed standard code to run
}

void setup() {
  SerialUSB.begin(230400);
  SerialUSB.print("Playing hooky\n");
  
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  greenirq.attach();//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  redirq.attach();
  flashLamp.restart(789);
}

void loop() {
  __WFE();
  if(rollovers){
    flashLamp.restart(300);
  }
}
