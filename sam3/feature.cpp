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

Feature::operator bool() const {
  return (*atAddress(base+8)&mask)!=0;
}
