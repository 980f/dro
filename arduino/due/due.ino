

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
  // put your setup code here, to run once:
  Show(HIGH);
  Show(LOW);
  Show(INPUT);
  Show(INPUT_PULLUP);
  Show(OUTPUT);
  Show(CHANGE); // to trigger the interrupt whenever the pin changes value
  Show(RISING); // to trigger when the pin goes from low to high,
  Show(FALLING);
}

#include "interruptPin.h"
void greenLight() {
  green = button2;
}

const InterruptPin<greenLight, button2.number, CHANGE> greenirq;


void setup() {
  SerialUSB.begin(230400);
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  greenirq.attach();//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
}

void loop() {
  // put your main code here, to run repeatedly:
  lamp = 1;
  red = button1;
  delay(750);
  lamp = 0;
  delay(1200);
  dump();
}
