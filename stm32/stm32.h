#ifndef stm32H
#define stm32H

/* stm family common stuff */
#include "eztypes.h"
#include "clocks.h"
/** @return bitband address for given bit (default 0) of given ram address*/
volatile u32 *bandFor(volatile void *dcb, unsigned int bitnum = 0);

///*
//  * bits are assigned via *name= boolean expression  else !!(non-boolean expression), the double bang turns a non-zero value into a 1 bit.
//  */
#define sfrbit(name, offset, bitoffset)  volatile unsigned int *name = reinterpret_cast <volatile unsigned *> (0x42000000 | ((offset) << 5) | ((bitoffset) << 2))

#ifdef __linux__  //faking it
extern u32 fakeram[2][1024];
#define APB_Block(bus2, slot) (&fakeram[bus2 != 0][0x10 * slot])
#define APB_Band(bus2, slot)   (&fakeram[bus2 != 0][0x100 * slot])
#else
#define APB_Block(bus2, slot) reinterpret_cast <u32 *> (0x40000000 | bus2 << 16 | slot << 10)
#define APB_Band(bus2, slot) reinterpret_cast <u32 *> (0x42000000 | bus2 << 21 | slot << 15)
#endif

//#def'd for the sake of (eventual) macro's to force inline allocations.
#ifdef __linuux__
#define RCCBASE pun(u32, fakeram)
#else
#define RCCBASE 0x40021000
#endif


/** each peripheral's reset pin, clock enable, and bus address are calculable from 2 simple numbers*/
struct APBdevice {
  const u8 bus2; //boolean, packed
  const u8 slot; //max 32 items (value is 0..31)
protected:
  /** @return bit address given the register address of the apb2 group*/
  u32 rccBase(unsigned int basereg) const {
    return RCCBASE + basereg + (bus2 ? 0 : 4);
  }
  /** @returns module base address */
  u32 *getAddress(void) const {
    return APB_Block(bus2, slot);
  }
  /** @returns module bit band base address */
  u32 *getBand(void) const {
    return APB_Band(bus2, slot);
  }

public:
  APBdevice(unsigned int abus, unsigned int aslot): bus2(abus - 1), slot(aslot){}

  APBdevice(const APBdevice &other): bus2(other.bus2), slot(other.slot){}
  /** activate and release the module reset */
  void reset(void) const;
  /** control the clock, off reduces chip power */
  void setClockEnable(bool on = true) const;
  /** @returns whether reset is off and clock is on */
  bool isEnabled(void) const;
  /** reset and enable clock */
  void init(void) const;
  /** get base clock for this module */
  u32 getClockRate(void) const;
  /** @returns address of a register, @param offset is value from st's manual (byte address) */
  u32 *registerAddress(unsigned int offset /*value as per st manual's byte address values*/) const {
    return getAddress() + (offset >> 2); //compiler sees offset as an array index .
  }
  /** @returns bit band address of bit of a register, @param offset is value from st's manual (byte address) */
  u32 *getBit(unsigned offset, unsigned bit){
    return APB_Band(bus2, slot)+(offset<<5)+(bit<<2);
  }

};
#endif /* ifndef stm32H */
