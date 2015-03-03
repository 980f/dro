#ifndef stm32H
#define stm32H

/* stm family common stuff */
#include "eztypes.h"
#include "clocks.h"
/** @return bitband address for given bit (default 0) of given ram address*/
//deprecated: volatile u32 *bandFor(volatile void *dcb, unsigned int bitnum = 0);

///*
//  * bits are assigned via *name= boolean expression  else !!(non-boolean expression), the double bang turns a non-zero value into a 1 bit.
//  */
#define sfrbit(name, offset, bitoffset)  volatile unsigned int *name = reinterpret_cast <volatile unsigned *> (0x42000000 | ((offset) << 5) | ((bitoffset) << 2))

#ifdef __linux__  //faking it
extern u32 fakeram[2][1024];
#define APB_Block(bus2, slot) (&fakeram[bus2 != 0][0x10 * slot])
#define APB_Band(bus2, slot)   (&fakeram[bus2 != 0][0x100 * slot])
#else
constexpr u32 * APB_Block(unsigned bus2, unsigned slot) { return reinterpret_cast <u32 *> (0x40000000 | bus2 << 16 | slot << 10);}
constexpr u32 * APB_Band(unsigned bus2, unsigned slot) { return reinterpret_cast <u32 *> (0x42000000 | bus2 << 21 | slot << 15);}

#endif

//#def'd for the sake of (eventual) macro's to force inline allocations.
#ifdef __linuux__
#define RCCBASE pun(u32, fakeram)
#else
#define RCCBASE 0x40021000
#endif


/** each peripheral's reset pin, clock enable, and bus address are calculable from 2 simple numbers.
working our way up to a template. */
struct APBdevice {
  const u8 bus2; //boolean, packed
  const u8 slot; //max 32 items (value is 0..31)
  /** base device address */
  u32 * const blockAddress;
  /** base bit band address */
  u32 * const bandAddress;
  /** base used for calculating this devices bits in RCC device. */
  u32  const rccBitter;

protected:
  /** @return bit address given the register address of the apb2 group*/
  u32 rccBase(unsigned int basereg) const {
    return rccBitter + basereg;
  }
  /** this class is cheap enough to allow copies, but why should we?: because derived classes sometimes want to be copied eg Port into pin).*/
  APBdevice(const APBdevice &other)=default;

public:
  /** @param bus and slot are per st documentation, hence a minus 1.*/
  APBdevice(unsigned int stbus, unsigned int slot):
    bus2(stbus - 1),
    slot(slot),
    blockAddress(APB_Block(bus2, slot)),
    bandAddress(APB_Band(bus2, slot)),
    rccBitter(RCCBASE + (bus2 ? 0 : 4))
  {}
  /** activate and release the module reset */
  void reset(void) const;
  /** control the clock, off reduces chip power */
  void setClockEnable(bool on = true) const;
  /** @returns whether clock is on */
  bool isEnabled(void) const;
  /** reset and enable clock */
  void init(void) const;
  /** get base clock for this module */
  u32 getClockRate(void) const;
  /** @returns address of a register, @param offset is value from st's manual (byte address) */
  u32 *registerAddress(unsigned int offset) const {
    return &blockAddress[offset >> 2]; //compiler sees offset as an array index .
  }
  /** @returns bit band address of bit of a register, @param offset is value from st's manual (byte address) */
  u32 *getBit(unsigned offset, unsigned bit){
    return &bandAddress[(offset<<5)+(bit<<2)];
  }
  /** @returns bit band address of bit of a register, @param offset is value from st's manual (byte address) */
  u32 &bit(unsigned offset, unsigned bit){
    return bandAddress[(offset<<5)+(bit<<2)];
  }
};
#endif /* ifndef stm32H */
