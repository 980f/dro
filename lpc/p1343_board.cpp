/*
 * P1343devkit.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: andyh
 */

#include "P1343devkit.h"
using namespace LPC;

P1343devkit::P1343devkit():
  button(PullUp){
  this->operator =(0xBD);
}

P1343devkit::~P1343devkit(){
  this->operator =(0); // lights off.
}

int P1343devkit::operator =(int lamp){
  // low nibble to port 3
  PortField<3, 3, 0> lownib;
  lownib = lamp;
  // high nibble to port 2
  PortField<2, 7, 4> highnib;
  highnib = lamp >> 4;
  return lamp;
}


Boolish &P1343devkit::led(unsigned which)
{
  switch(which){
  default:
  case 0: return led0;
  case 1: return led1;
  case 2: return led2;
  case 3: return led3;
  case 4: return led4;
  case 5: return led5;
  case 6: return led6;
  case 7: return led7;
  }

}
