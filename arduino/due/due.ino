

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


#define Show(arg) SerialUSB.print("\n" #arg ":");SerialUSB.print(arg)
void dump() {
 
}

#include "interruptPin.h"
void greenLight() {
  green = button2;
  RX=green;
}

void redLight(){
  red = button1;
  TX=red;
}

const InterruptPin<redLight, button1.number, CHANGE> redirq;

const InterruptPin<greenLight, button2.number, CHANGE> greenirq;
  
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
  PolledTimerServer();//all of our millisecond stuff hangs off of here.
  return false; //allowed standard code to run
}

void setup() {
  SerialUSB.begin(230400);//'native' usb port
  SerialUSB.print("Playing hooky\n");
  Serial.begin(115200);//an actual uart
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  greenirq.attach(true);//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  redirq.attach();
  flashLamp.restart(789);
}

char indicator='-';
void loop() {
  __WFE();
  Serial.write(indicator);
}
