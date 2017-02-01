#ifndef ARDUINODUE_H
#define ARDUINODUE_H

#include "gpio.h"

class ArduinoDue{
public:
  ArduinoDue();
/** seeed studio protoshield rev A*/
//D3-red,D4-green  leds buttons D6,D7
  OutputPin red;       //D3=
  OutputPin green;
  InputPin button1;
  InputPin button2;
  OutputPin D13; //also an LED?

};

#endif // ARDUINODUE_H
