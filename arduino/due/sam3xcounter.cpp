#include "sam3xcounter.h"

#include "peripheraltypes.h"
#include "peripheralband.h"

/*
IRQ: 27+MODULE

to synch all the counters one can use the capture feature, then leasurely trigger them in sets of 3 using SYNC.
That would leave a small skew, but with asm that can be at most 2 and 4 processor clocks.

*/


static constexpr unsigned tcBase(unsigned tmod,unsigned tcnum){
  return (0x40080000) | (tmod<<14) | (tcnum<<6);
}

static constexpr unsigned tcBase(unsigned tc1of9){
  return tcBase(tc1of9/3,tc1of9%3);
}


CountPin::CountPin(unsigned tc1of9, unsigned arduinoPinNumber):
  tcu(tc1of9%3),
  tmod(tc1of9/3),
  base(tcBase(tmod,tcu)){
  configure(arduinoPinNumber);
}

void CountPin::configure(unsigned arduinoPinNumber) const {
  //5+xc#
}

void CountPin::start(bool withReset) const{
  *atAddress(base)=withReset?5:1;
}

void CountPin::stop() const{
  *atAddress(base)=2;
}


CountPin::operator unsigned() const{
  return *atAddress(base+0x10);
}

unsigned CountPin::roll() const{
  unsigned was=*atAddress(base+0x10);
  start(true);
  return was;
}

