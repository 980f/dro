#ifndef LPCPERIPHERAL_H
#define LPCPERIPHERAL_H

#include "peripheraltypes.h"

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
//  constexpr unsigned FLASH_BASE(0x00000000UL);
//  constexpr unsigned RAM_BASE(0x10000000UL);
namespace LPC {

// 16k blocks of address space are allocated per apb device
constexpr unsigned apb0Device(unsigned unitNumber){
  return 0x40000000UL + (unitNumber << 14);
}

/** address for SFRfield and bit for syscon reg given offset listed in manual */
constexpr unsigned sysConReg(unsigned byteOffset){
  return apb0Device(18)+byteOffset;
}

}
#define DefineSingle(regname, addr) LPC:: regname & the ## regname(*reinterpret_cast<LPC:: regname *>(addr))

//// this variation is for things like  multiple functionally identical timers:
#define DefineUnit(regname, luno, addr) LPC:: regname * const the ## regname ## luno(reinterpret_cast<LPC:: regname *>(addr))


#endif // LPCPERIPHERAL_H
