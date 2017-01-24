#include "p1343_board.h"

#include "clocks.h"

const u32 EXTERNAL_HERTZ=12000000;

using namespace LPC;

P1343devkit::P1343devkit(){
//  button.setIocon(PullUp);
  this->operator =(0xB9);
}


P1343devkit::~P1343devkit(){
  this->operator =(0); // lights off.
}

unsigned P1343devkit::operator =(unsigned lamp)const {
  lownib = lamp;
  highnib = lamp >> 4;
  return lamp;
}

void P1343devkit::toggleLed(unsigned which)const {
  const BoolishRef &anLed(led(which));
  anLed=1-anLed;
}


const BoolishRef &P1343devkit::led(unsigned which)const {
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

