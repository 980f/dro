#ifndef LPCPERIPHERAL_H
#define LPCPERIPHERAL_H

#include "peripheraltypes.h"

namespace LPC {

/** 16k blocks of address space are allocated per apb device */
constexpr unsigned apb0Device(unsigned unitNumber){
  return 0x40000000UL + (unitNumber << 14);
}

/** a control register given documentation of its offset */
constexpr unsigned makeSFR(unsigned apbdev, unsigned offset){
  return apb0Device(apbdev) + offset;
}

/** address for syscon reg given offset listed in manual */
constexpr unsigned &sysConReg(unsigned byteOffset){
  return atAddress(makeSFR(18,byteOffset));
}

inline void powerUp(int which){
  clearBit(sysConReg(0x238),which);
}

inline void powerDown(int which){
  setBit(sysConReg(0x238),which);
}

/** iocon group */
constexpr unsigned ioConReg(unsigned byteOffset){
  return makeSFR(17,byteOffset);
}

/** enable the selected device clock. Since each will only be referenced in its own module there is no need for formal enum.  */
inline void enableClock(unsigned bit){
  setBit(sysConReg(0x80), bit);
}

/** enable the selected device clock. Since each will only be referenced in its own module there is no need for formal enum.  */
inline void disableClock(unsigned bit){
  clearBit(sysConReg(0x80), bit);
}


/** there are only 3 items with resets: 0: ssp0, 1:I2c 2:ssp1 */
inline void reset(unsigned bit){
  raiseBit(sysConReg(4), bit);
}


} // namespace LPC


struct TestInitSequence {
  static void setLevel(int &level){
    ++level;
  }

  TestInitSequence(int myLevel){
    setLevel(myLevel);
  }
};

#define DefineSingle(regname, addr) LPC:: regname & the ## regname(*reinterpret_cast<LPC:: regname *>(addr))

////// this variation is for things like  multiple functionally identical timers:
//#define DefineUnit(regname, luno, addr) LPC:: regname * const the ## regname ## luno(reinterpret_cast<LPC:: regname *>(addr))


#endif // LPCPERIPHERAL_H
