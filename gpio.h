#ifndef GPIO_H
#define GPIO_H

/*****************************************************************************
 *   gpio.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   c++ version of LPC stuff, thoroughly typed
 * and then a purely c++ performance tweaked version (operations 100% inlinable)
 ******************************************************************************/

#include "lpcperipheral.h"

/** the ports are numbered from 0. Making them unsigned gives us a quick bounds check via a single compare*/
typedef unsigned PortNumber;

/** we could constrain this for 0..11 */
typedef unsigned BitNumber;

namespace LPC {

constexpr bool isLegalPort(PortNumber pn){
  return pn < 4;
}

constexpr uint32_t portBase(PortNumber portNum){
  return 0x50000000 + (portNum << 16); //this is the only ahb device, and each gpio is 4 blocks thereof so just have a custom address computation.
}

/** in reality there are ~40 io pins.
 this index is useful for things like figuring out which interrupt vector is associated with the pin. */
constexpr int pinIndex(PortNumber portNum, BitNumber bitPosition){
  return portNum*12+bitPosition;
}

/** there is no relationship between the ioconfiguration register for a pin and its gpio designation.
 the LPC designers should be spanked for this, spanked hard and with something nasty. */
constexpr int ioconf_map[]= {
  3, 4, 7,11,12,13,19,20,24,25,26,29,
  30,31,32,36,37,40,41,42, 5,14,27,38,
  2,10,23,35,16,17, 0, 8, 9,21,22,28,
  33,34,39,43,15,18,
  //1 6 are not present in this list (reserved locations in hardware map), gok what they may be used for someday.
};

constexpr int ioconIndex(PortNumber portNum, BitNumber bitPosition){
  return ioconf_map[pinIndex(portNum,bitPosition)];
}
//weird iocons: 0.0 1 rather than 0 for gpio, is reset* so normally not used for gpio.

//there are 4 iocon registers that aren't directly related to a gpio designator.

/** traditional verbose way of doing I/O, but not nearly so verbose as the nxp cmsis version it mimics.
 * instances of this class act like a boolean variable but call functions to do access operations.
 * the template GpioPin class generates fully inlined code instead of calling convenience functions.
 * If the static methods in this class were inlined you might get the same degree of inlining to occur.
 * */
class GPIO {
private:
  const PortNumber portNumber;
  const BitNumber bitNumber;
public:
  GPIO(PortNumber portNum, BitNumber bitPosition):
    portNumber(portNum),
    bitNumber(bitPosition){
  }
  bool operator =(bool setHigh) const {
    SetValue(portNumber, bitNumber, setHigh);
    return setHigh;
  }
  operator bool() const {
    return GetValue(portNumber, bitNumber);
  }
public:
  /** this must be called once before any other functions herein are used. SystemInit is a good place, so that constructors for outputs will work.*/
  static void Init( void );
public: // cmsis/ lpcopen legacy;
  static void SetDir( PortNumber portNum, BitNumber bitPosi, bool outputter);
  static void SetValue( PortNumber portNum, BitNumber bitPosi, bool bitVal );
  /** @returns actual pin value, not necessarily last value set. */
  static bool GetValue( PortNumber portNum, BitNumber bitPosi);

  static void SetInterrupt( PortNumber portNum, BitNumber bitPosi, uint32_t sense, uint32_t single, uint32_t event );
  static void IntEnable( PortNumber portNum, BitNumber bitPosi );
  static void IntDisable( PortNumber portNum, BitNumber bitPosi );
  static bool IntStatus(PortNumber portNum, BitNumber bitPosi );
  static void IntClear( PortNumber portNum, BitNumber bitPosi );
};

//and now for the modern approach:
/** to configure a pin for a dedicated function one must merely construct a GpioPin with template args for which pin and constructor arg of control pattern*/
template <PortNumber portNum, BitNumber bitPosition> class GpioPin {
protected: //for simple gpio you must use an extended class that defines read vs read-write capability.
  enum {
    mask= 1<<bitPosition,   //used for control register access
    base=portBase(portNum), //base for control register
    mode=ioconIndex(portNum,bitPosition), //iocon array index
    pinn=base+(mask<<2)    //phsyical pin 'masked' access location
  };

  /** set associated IOCON register to @param pattern.
   * Each pin has its own rules as to what the pattern means, although there are a large set of common patterns. */
  inline void setIocon(uint32_t pattern){
    //this presumes that the IOCON has been globally enabled by systeminit
    reinterpret_cast<uint32_t*>(LPC::apb0Device(17))[mode]=pattern;
  }

  inline GpioPin(uint32_t pattern){
    setIocon(pattern);
  }
  /** @returns reference to the masked access port of the register, mask set to the one bit for this pin. @see InputPin and @see OutputPin classes for use, unlike stm32 bitbanding some shifting is still needed. */
  inline uint32_t& pin()const {
    return *reinterpret_cast<uint32_t*>(GpioPin<portNum, bitPosition>::pinn);
  }
};

/** declared outside of InputPIn class so that we don't have to apply template args to each use.*/
enum InputPinBias {
  LeaveFloating=0, //in case someone forgets to explicitly select a mode
  BusLatch,   //edge, either edge, input mode buslatch
  PullUp, //level, pulled up
  PullDown, //level, pulled down
};

/** simple digital input */
template <PortNumber portNum, BitNumber bitPosition> class InputPin:public GpioPin<portNum, bitPosition> {
private:
  void operator =(bool); //private because this is a read-only entity.

public:

  static constexpr unsigned ioconPattern(InputPinBias bias){
    //todo: coerce making it an input. Not simple as some gpio pins have inverted configuration from the bulk.
    return bias;
  }

  /** @param yanker controls pullup modality */
  InputPin(InputPinBias yanker=BusLatch):GpioPin<portNum, bitPosition>(ioconPattern(yanker)){
    //nothing to do.
  }
  /** use the pin as if it were a boolean variable. */
  inline operator bool() const {
    return GpioPin<portNum, bitPosition>::pin()!=0;//need to check assembler, a shift might be better.
  }
};

/** simple digital output */
template <PortNumber portNum, BitNumber bitPosition> class OutputPin:public GpioPin<portNum, bitPosition> {
public:
  /** @param yanker controls pull-up modality */
  OutputPin(int yanker=0):GpioPin<portNum, bitPosition>(yanker){
    //todo: coerce making it an input.
  }
  /** use the pin as if it were a boolean variable. */
  inline operator bool() const {
    return GpioPin<portNum, bitPosition>::pin()!=0;
  }

  bool operator =(bool newvalue){
    GpioPin<portNum, bitPosition>::pin()=newvalue?~0:0;//don't need to mask or shift, just present all ones or all zeroes and let the hardware 'mask with address' take care of business.
    return newvalue;
  }
};

/** Multiple contiguous bits in a register, this presumes the bits are configured elsewhere via GpioPin objects,
 * this class just expedites access using the gpio port 'masked[]' based access
 */
template <PortNumber portNum, int msb, int lsb> class PortField {
  enum {
    //read the lpc manual, certain address bits are used as a mask
    sfraddress= portBase(portNum) + ((1<<(msb+3))-(1<<(lsb+2)))
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
    *sfr() = value << lsb ;
  }
};


/** interrupt input */
template <PortNumber portNum, BitNumber bitPosition> class IrqPin:public GpioPin<portNum, bitPosition> {
private:
  void operator =(bool); //because this is a read-only entity.
  /** use the pin as if it were a boolean variable. */
  inline operator bool() const {
    return GpioPin<portNum, bitPosition>::pin()!=0;
  }
public:
  //values for gpio config as well as irq config.
  enum IrqStyle {
    NotAnInterrupt=0, //in case someone forgets to explicitly select a mode
    AnyEdge,   //edge, either edge, input mode buslatch
    LowActive, //level, pulled up
    HighActive, //level, pulled down
    LowEdge,   //edge, pulled up
    HighEdge,  //edge, pulled down

  };

  /** @param yanker controls pullup modality */
  IrqPin(int yanker):GpioPin<portNum, bitPosition>(yanker){
    //todo: set multiple registers
  }
  //todo: functions for dynamic inspection and enabling. //might allow for dynamic redefinition of polarity.
};
}
#endif
