#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "eztypes.h"
/** things that are handy for describing peripherals */

//type converter
constexpr u32* atAddress(u32 address){
  return reinterpret_cast<u32 *>(address);
}

/** for a private single instance block */
#define soliton(type, address) type & the ## type = *reinterpret_cast <type *> (address);


/** @return bitband address for given bit (default 0) of given ram address*/
//deprecated:
volatile u32 *bandFor(volatile void *dcb, unsigned int bitnum = 0);

const u32 stmPeripheralBase(0x40000000);

const u32 stmPeripheralBand(0x42000000);


#endif // PERIPHERAL_H
