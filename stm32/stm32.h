#ifndef stm32H
#define stm32H

/* stm family common stuff */
#include "eztypes.h"
#include "clocks.h"
#include "stm32/peripheral.h" //too many files have almost the same name so we force a directory here so include path doesn't need to be carefully ordered.

#ifdef __linux__  //faking it
extern u32 fakeram[2][1024];
#define APB_Block(bus2, slot) (&fakeram[bus2 != 0][0x10 * slot])
#define APB_Band(bus2, slot)   (&fakeram[bus2 != 0][0x100 * slot])
#else
constexpr u32 * APB_Block(unsigned bus2, unsigned slot) { return atAddress(stmPeripheralBase | bus2 << 16 | slot << 10);}//extracted from stm'2 doc
constexpr u32 * APB_Band(unsigned bus2, unsigned slot)  { return atAddress(stmPeripheralBand | bus2 << 21 | slot << 15);}//bit band ~ 32 * base
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
  u32 const rccBitter;

protected:
  /** @return bit address given the register address of the apb2 group*/
  inline u32 &rccBit(unsigned basereg) const {
    return *reinterpret_cast<u32 *>(rccBitter | (basereg<<5));
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
    rccBitter(stmPeripheralBand | (RCCBASE | (bus2 ? 0 : 4))<<5) //bus 2 is the first of a pair of which bus1 is the second. ST doesn't like consistency.
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
    return &blockAddress[offset>>2]; //compiler sees offset as an array index .
  }
  /** @returns bit band address of bit of a register, @param offset is value from st's manual (byte address) */
  u32 *getBit(unsigned offset, unsigned bit)const{
    return &bandAddress[(offset<<3)+bit];
  }
  /** @returns bit band address of bit of a register, @param offset is value from st's manual (byte address) */
  u32 &bit(unsigned offset, unsigned bit)const{
    return bandAddress[(offset<<3)+bit];//compiler adds another shift of 2 places
  }
};
#endif /* ifndef stm32H */
