/*
 * P1343devkit.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: andyh
 */

#include "P1343devkit.h"
using namespace LPC;

P1343devkit::P1343devkit():
button(PullUp)
{
  this->operator =(0xBD);
}

P1343devkit::~P1343devkit(){
  this->operator =(0);//lights off.
}

int P1343devkit::operator =(int lamp){
  //low nibble to port 3
  PortField<3,3,0> lownib;
  lownib=lamp;
  //high nibble to port 2
  PortField<2,7,4> highnib;
  highnib=lamp>>4;
  return lamp;
}
