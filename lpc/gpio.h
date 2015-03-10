#ifndef GPIO_H
#define GPIO_H

/*****************************************************************************
 *   gpio.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   c++ version of LPC stuff, thoroughly typed
 * and then a purely c++ performance tweaked version (operations 100% inlinable)
 ******************************************************************************/

#include "lpcperipheral.h"
#include "cheapTricks.h"
#include "boolish.h"

/** the ports are numbered from 0. Making them unsigned gives us a quick bounds check via a single compare*/
typedef u8 PortNumber;

/** we could constrain this for 0..11 */
typedef u8 BitNumber;

namespace LPC {

constexpr bool isLegalPort(PortNumber pn){
  return pn < 4;
}

/** @returns block base address, 64k addresses per port */
constexpr uint32_t portBase(PortNumber portNum){
  return 0x50000000 + (portNum << 16); // this is the only ahb device, and each gpio is 4 blocks thereof so just have a custom address computation.
}

/** @returns linear index of pin (combined port and bit)
 *  this index is useful for things like figuring out which interrupt vector is associated with the pin. */
constexpr int pinIndex(PortNumber portNum, BitNumber bitPosition){
  return portNum * 12 + bitPosition;
}

/** there is no relationship between the ioconfiguration register for a pin and its gpio designation.
 *  the LPC designers should be spanked for this, spanked hard and with something nasty. */
constexpr int ioconf_map[] = {
  3, 4, 7, 11, 12, 13, 19, 20, 24, 25, 26, 29,
  30, 31, 32, 36, 37, 40, 41, 42, 5, 14, 27, 38,
  2, 10, 23, 35, 16, 17, 0, 8, 9, 21, 22, 28,
  33, 34, 39, 43, 15, 18,
  // 1 6 are not present in this list (reserved locations in hardware map), gok what they may be used for someday.
};

// constexpr int ioconIndex(PortNumber portNum, BitNumber bitPosition){
//  return ioconf_map[pinIndex(portNum,bitPosition)];
// }
// weird iocons: 0.0 1 rather than 0 for gpio, is reset* so normally not used for gpio.

// there are 4 iocon registers that aren't directly related to a gpio designator.
#if 0
SFR SCK_LOC; /*!< Offset: 0x0B0 SCK pin location select Register (R/W) */
#endif

/** for access to the 4 non-gpio pin registers:*/
constexpr unsigned ioconRegister(unsigned offset){
  return LPC::apb0Device(17) + offset;
}

/** express access to a pin.
 * will add field access objects when that proves useful.
 */
class GPIO :public BoolishRef {
private:
  /** address associated with single bit mask */
  u32 & dataAccess;

public:
  GPIO(PortNumber portNum, BitNumber bitPosition):
    dataAccess(*reinterpret_cast<u32 *>(portBase(portNum)+((1<<bitPosition)<<2)))
  {/*empty*/ }

  bool operator =(bool setHigh) const {
    dataAccess = 0-setHigh;//all ones for setHigh, all zeroes for !setHigh, address picks the bit.
    return setHigh;
  }

  operator bool() const {
    return dataAccess!=0;
  }

public:
  /** this must be called once before any other functions herein are used. SystemInit is a good place, so that constructors for outputs will work.*/
  static void Init( void );

};


/** declared outside of InputPIn class so that we don't have to apply template args to each use.*/
enum PinBias { //#ordered for MODE field of iocon register
  LeaveFloating = 0, // in case someone forgets to explicitly select a mode
  PullDown, // level, pulled down
  PullUp, // level, pulled up
  BusLatch, // edge, either edge, input mode buslatch

};

class GPIOConfigurator {
  const PortNumber portNumber;
  const BitNumber bitNumber;
  GPIOConfigurator(PortNumber portNumber, BitNumber bitNumber):
   portNumber(portNumber),
   bitNumber(bitNumber){
 }

};



// and now for the modern approach:
/** to configure a pin for a dedicated function one must merely construct a GpioPin with template args for which pin and constructor arg of control pattern*/
template <PortNumber portNum, BitNumber bitPosition> class GpioPin: public BoolishRef {
public:
  enum {
    pini = pinIndex(portNum, bitPosition)
};

protected: // for simple gpio you must use an extended class that defines read vs read-write capability.
  enum {
    mask = 1 << bitPosition, // used for port control register access
    base = portBase(portNum), // base for port control
    // some pins are special on reset
    doa = (portNum == 0 && (bitPosition == 0 || bitPosition == 10 || bitPosition == 11)) || // 0.0 0.10 0.11
    (portNum == 1 && (bitPosition < 4)), // 1.0 1.1 1.2 1.3
    mode = ioconf_map[pini], // iocon array index
    pinn = base + (mask << 2)    // physical pin 'masked' access location "address == pattern"
  };

  /** set associated IOCON register to @param pattern.
     * Each pin has its own rules as to what the pattern means, although there are a large set of common patterns. */
  inline void setIocon(uint32_t pattern){
    // this presumes that the IOCON has been globally enabled by systeminit
    reinterpret_cast<uint32_t *>(LPC::apb0Device(17))[mode] = pattern;
  }

  /** @returns reference to the masked access port of the register, mask set to the one bit for this pin. @see InputPin and @see OutputPin classes for use, unlike stm32 bitbanding some shifting is still needed. */
  inline uint32_t &pin() const {
    return *reinterpret_cast<uint32_t *>(GpioPin<portNum, bitPosition>::pinn);
  }
public:

  /** only special pins should use this directly. */
  inline GpioPin(uint32_t pattern){
    setIocon(pattern);
  }

  // biasing is independent of in vs out, but not of function.
  static constexpr unsigned ioconPattern(PinBias bias){
    // FYI 0.4 and 0.5 reset to 0, all others to D0
    //  2 bits are the code passed in
    //   ls 3 bits are either 0 for normal pins or 1 for doa pins like reset or SWD pins.
    return (1<<7) | (1 << 6) | (bias << 3) | doa;
    // ignoring hysteresis option, it depends upon VDD so we'd have to define a symbol for that.
  }

  static constexpr unsigned analogInputPattern(){
    // the doa bits get a 2 rather than a 1 to select analog functionality
    // we disable pullups and pulldowns on analog channels (bias==0)
    // bit 7 is a zero for analog selection
    return (1 << 6) | (doa?2:1);
  }

  /** use the pin as if it were a boolean variable. */
  inline operator bool() const {
    return GpioPin<portNum, bitPosition>::pin() != 0; // need to check assembler, a shift might be better.
  }

};


/** simple digital input */
template <PortNumber portNum, BitNumber bitPosition> class InputPin: public GpioPin<portNum, bitPosition> {
private:
  bool operator =(bool)const {return false;} // private because this is a read-only entity.

public:
  /** @param yanker controls pullup modality */
  InputPin(PinBias yanker = BusLatch): GpioPin<portNum, bitPosition>(this->ioconPattern(yanker)){
    // nothing to do.
  }

};

/** simple digital output */
template <PortNumber portNum, BitNumber bitPosition> class OutputPin: public GpioPin<portNum, bitPosition>{
public:
  /** @param yanker controls pull-up modality */
  OutputPin(PinBias yanker = BusLatch): GpioPin<portNum, bitPosition>(this->ioconPattern(yanker)){
    // todo: coerce making it an input.
  }

  bool operator =(bool newvalue)const{
    GpioPin<portNum, bitPosition>::pin() = newvalue ? ~0 : 0; // don't need to mask or shift, just present all ones or all zeroes and let the hardware 'mask with address' take care of business.
    return newvalue;
  }
};

/** Multiple contiguous bits in a register, this presumes the bits are configured elsewhere via GpioPin objects,
 * this class just expedites access using the gpio port 'masked[]' based access
 */
template <PortNumber portNum, int msb, int lsb> class PortField {
  enum {
    // read the lpc manual, certain address bits are used as a mask
    sfraddress = portBase(portNum) + ((1 << (msb + 3)) - (1 << (lsb + 2)))
  };

  inline SFR *sfr() const {
    return reinterpret_cast<SFR *>(sfraddress);
  }
public:
  // read
  inline operator uint32_t() const {
    return *sfr() >> lsb;
  }
  // write
  inline void operator =(uint32_t value) const {
    *sfr() = value << lsb;
  }
};

// values for gpio config as well as irq config.
enum IrqStyle {
  NotAnInterrupt = 0, // in case someone forgets to explicitly select a mode
  AnyEdge, // edge, either edge, input mode buslatch
  LowActive, // level, pulled up
  HighActive, // level, pulled down
  LowEdge, // edge, pulled up
  HighEdge   // edge, pulled down
};


/** interrupt input */
template <PortNumber portNum, BitNumber bitPosition> class IrqPin: public GpioPin<portNum, bitPosition> {
private:
  void operator =(bool); // because this is a read-only entity.
  /** use the pin as if it were a boolean variable. */
  inline operator bool() const {
    return GpioPin<portNum, bitPosition>::pin() != 0;
  }
  //                                    NotAnInterrupt,AnyEdge, LowActive, HighActive, LowEdge, HighEdge
  static constexpr unsigned biasForIrqStyle[] = { BusLatch, BusLatch, PullUp, PullDown, PullUp, PullDown };

public:

  /** @param irqStyle controls bias as well as configuring interruptness */
  IrqPin(IrqStyle irqStyle): GpioPin<portNum, bitPosition>(ioconPattern(biasForIrqStyle[irqStyle % 6])){
    // todo: set multiple registers
  }
  // todo: functions for dynamic inspection and enabling. //might allow for dynamic redefinition of polarity.
};
} // namespace LPC
#endif // ifndef GPIO_H
