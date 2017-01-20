#pragma once

#include "gpio.h" // to declare buttons and lamps.

namespace LPC {

//portIndex(2,9) is irq for button
#define P1343ButtonIrqNum 33

class P1343devkit {
public:
  //push button express access
  const LPC::InputPin<2, 9> button;
  //push button full function access, same physical component as @see button.
  const LPC::GPIO but1;
  //other button is 'wakeup'
  //reset button is pio0/0, if reset functionality is defeated.
  const LPC::OutputPin<3, 0> led0;
  const LPC::OutputPin<3, 1> led1;
  const LPC::OutputPin<3, 2> led2;
  const LPC::OutputPin<3, 3> led3;
  const LPC::OutputPin<2, 4> led4;
  const LPC::OutputPin<2, 5> led5;
  const LPC::OutputPin<2, 6> led6;
  const LPC::OutputPin<2, 7> led7;

  //parallel access to the leds
  const LPC::PortField<3, 3, 0> lownib;
  const LPC::PortField<2, 7, 4> highnib;

  P1343devkit();
  ~P1343devkit();
  /** set lamps as an 8-bit number, not particularly swift in execution since they are scattered about the i/o space*/
  int operator =(int lamp )const;
  /** set led by ordinal.*/
  const BoolishRef &led(unsigned which)const ;
  /** invert state of one led */
  void toggleLed(unsigned which=0)const ;
};
}
