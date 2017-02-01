

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
}

void setup() {
  SerialUSB.begin(230400);
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraryies.
}

void loop() {
  // put your main code here, to run repeatedly:
  lamp = 1;
  red=button1;
  green=button2;
  delay(750);
  lamp = 0;
  delay(1200);
  dump();
}
