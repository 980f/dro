#include "feature.h"
#include "peripheraltypes.h"

using namespace SAM;

Feature::Feature(unsigned base, unsigned mask, bool invertedSense):
  base(base),
  mask(mask),
  invertedSense(invertedSense){
  //#nada
}

void Feature::operator =(bool enableit) const{
  if(enableit!=invertedSense){
    *atAddress(base)=mask;
  } else {
    *atAddress(base+4)=mask;
  }
}

unsigned &Feature::fourth() const {
  return *atAddress(base+3*4);
}

void Feature::set4th(bool on){
  unsigned address=base+3*4;
  if(on){
    *atAddress(address)|=mask;
  } else {
    *atAddress(address)&=~mask;
  }
}

bool Feature::get4th(){
  return bit(*atAddress(base+3*4),mask);
}

Feature::operator bool() const {
  return (*atAddress(base+2*4)&mask)!=0;
}

///////////////////////////////////

/** many registers can only be written to when not protected. */
void protectControlRegisters(bool on){
  *atAddress(0x400E04E4)= (0x504D43<<8) | on;
}
