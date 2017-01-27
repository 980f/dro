#ifndef SAMPERIPHERAL_H
#define SAMPERIPHERAL_H

#include "peripheraltypes.h"

constexpr unsigned samblock(unsigned chunkNumber){
  return PeripheralBase+(chunkNumber<<14);
}


//blocks of 0x200 starting at the group 24 or so:
constexpr unsigned scmbase(unsigned chunkNumber){
  return samblock(24)+(chunkNumber<<9);
}


#endif // SAMPERIPHERAL_H
