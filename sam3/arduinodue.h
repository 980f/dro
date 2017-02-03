#ifndef ARDUINODUE_H
#define ARDUINODUE_H

#include "gpio.h"

class ArduinoDue{
public:
//  ArduinoDue();
/** seeed studio protoshield rev A*/
  OutputPin<PC,28> red;       //D3=
  OutputPin<PA,29> green;  //also PC26??? two device pins tied to one connection?
  InputPin<PC,24> button1;
  InputPin<PC,23> button2;
/** standard arduino LED*/
  OutputPin<PB,27> LED; //aka D13


};

#endif // ARDUINODUE_H
