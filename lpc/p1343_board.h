#pragma once

#include "gpio.h" // to declare buttons and lamps.

namespace LPC {

class P1343devkit {
public:
  const LPC::InputPin<2,9> button;//={2,9};
  //other button is 'wakeup'
  const LPC::InputPin<1,4> wakeup;
  //reset button is pio0/0, if reset functionality is defeated.
  //pin 1/0 is bootup select, avoid hanging anything important on it.
  const LPC::Output led0={3, 0};
  const LPC::Output led1={3, 1};
  const LPC::Output led2={3, 2};
  const LPC::Output led3={3, 3};
  const LPC::Output led4={2, 4};
  const LPC::Output led5={2, 5};
  const LPC::Output led6={2, 6};
  const LPC::Output led7={2, 7};

  //parallel access to the leds
  const GpioOutputField lownib={3, 3, 0};
  const GpioOutputField highnib={2, 7, 4};

  P1343devkit();
  ~P1343devkit();
  /** set lamps as an 8-bit number, not particularly swift in execution since they are scattered about the i/o space*/
  unsigned operator =(unsigned lamp )const;
  /** set led by ordinal.*/
  const BoolishRef &led(unsigned which)const ;
  /** invert state of one led */
  void toggleLed(unsigned which=0)const ;
};
}

#if 0
UEXt connector pinout:
  3.3  gnd
  txd  rxd
  scl  sda
  miso mosi
  sck  ssel
#endif
