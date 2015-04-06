#pragma once

#include "gpio.h" // to declare buttons and lamps.

namespace LPC {

/** this class exists to trigger timely execution of initialization routines since we only have figured out how to call things in order by setting attributes on objects to be statically constructed. */
struct Board {
  Board(){
    GPIO::Init();//this must run before any pins can be reliably be statically created.
  }
};
}

//portIndex(2,9) is irq for button
#define P1343ButtonIrqNum 33
class P1343devkit:public LPC::Board {
public:
  //push button express access
  LPC::InputPin<2, 9> button;
  //push button full function access, same physical component as @see button.
  LPC::GPIO but1;
  //other button is 'wakeup'
  //reset button is pio0/0, if reset functionality is defeated.
  LPC::OutputPin<3, 0> led0;
  LPC::OutputPin<3, 1> led1;
  LPC::OutputPin<3, 2> led2;
  LPC::OutputPin<3, 3> led3;
  LPC::OutputPin<2, 4> led4;
  LPC::OutputPin<2, 5> led5;
  LPC::OutputPin<2, 6> led6;
  LPC::OutputPin<2, 7> led7;

  //parallel access to the leds
  LPC::PortField<3, 3, 0> lownib;
  LPC::PortField<2, 7, 4> highnib;

  P1343devkit();
  ~P1343devkit();
  /** set lamps as an 8-bit number, not particularly swift in execution since they are scattered about the i/o space*/
  int operator =(int lamp );
  /** set led by ordinal.*/
  BoolishRef &led(unsigned which);
  /** invert state of one led */
  void toggleLed(unsigned which=0);
};
