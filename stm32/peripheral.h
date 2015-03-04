#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "eztypes.h"
/** things that are handy for describing peripherals
volatile is used to keep the compiler from optimizing away accesses that have physical side effects.
I am working on replacing *'s with &'s, its a statisitcal thing herein as to which is better.
*/

/** @returns byte address argument as a pointer to that byte */
inline constexpr volatile u32* atAddress(u32 address){
  return reinterpret_cast<u32 *>(address);
}

/** for a private single instance block */
#define soliton(type, address) type & the ## type = *reinterpret_cast <type *> (address);


inline constexpr u32 bandShift(u32 byteOffset){
  return byteOffset<<5;
}

/** @return bitband address for given bit (default 0) of @param byte address.
this assumes that the byte address ends in 00, which all of the ones in the st manual do.
*/
inline constexpr volatile u32 *bandFor(u32 byteAddress, unsigned bitnum = 0){
  //bit gets shifted by 2 as the underlying mechanism inspects the 32bit word address only, doesn't see the 2 lsbs of the address.
  //5: 2^5 bits per 32 bit word. we expect a byte address here, so that one can pass values as read from the stm32 manual.
  //0xE0000000: stm32 segments memory into 8 512M blocks, banding is within a block
  //0x02000000: indicates this is a bitband address
  //bit to lsbs of address |  byteaddress shifted up far enouhg for address space to go away | restore address space | bitband indicator.
  return atAddress ((bitnum << 2) | bandShift(byteAddress) | (byteAddress & 0xE0000000) | 0x02000000);
}

/** @return bitband address for given bit (default 0) of @param byte address.
this assumes that the byte address ends in 00, which all of the ones in the st manual do.
*/
inline constexpr volatile u32 *bandAddress(volatile u32 * byteAddress, unsigned bitnum = 0){
  return bandFor(u32(byteAddress),bitnum);
}

//the following should be deprecated, subsumed in careful use of bandFor
const u32 stmPeripheralBase(0x40000000);

const u32 stmPeripheralBand(0x42000000);


#endif // PERIPHERAL_H
