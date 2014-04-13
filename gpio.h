#ifndef GPIO_H
#define GPIO_H

/*****************************************************************************
 *   gpio.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   c++ version of LPC stuff, thoroughly typed
 * and then a purely c++ performance tweaked version (operations 100% inlinable)
 ******************************************************************************/

#include "peripheraltypes.h"
#include "lpc13xx.h" //for base address stuff

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
 //1 6 are empty.
};

constexpr int ioconIndex(PortNumber portNum, BitNumber bitPosition){
  return ioconf_map[pinIndex(portNum,bitPosition)];
}
//weird iocons: 0.0 1 rather than 0 for gpio, is reset* so normally not used for gpio.

//there are 4 iocon registers that aren't directly related to a gpio designator.

/** traditional verbose way of doing I/O, but not nearly so verbose as the nxp cmsis version it mimics.*/
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
protected:
  enum {
    mask= 1<<bitPosition,   //used for control register access
    base=portBase(portNum), //base for control register
    mode=ioconIndex(portNum,bitPosition), //iocon array index
    pinn=base+(mask<<2)    //phsyical pin 'masked' access location
  };
  //each pin has its own rules as to what the pattern means, although there are a large set of common patterns.
  inline GpioPin(uint32_t pattern){
    reinterpret_cast<uint32_t*>(apb0Device(17))[mode]=pattern;
  }
  inline uint32_t& pin()const {
    return *reinterpret_cast<uint32_t*>(GpioPin<portNum, bitPosition>::pinn);
  }
};

/** simple digital input */
template <PortNumber portNum, BitNumber bitPosition> class InputPin:public GpioPin<portNum, bitPosition> {
private:
  void operator =(bool); //because this is a read-only entity.
public:
  /** @param yanker controls pullup modality */
  InputPin(int yanker):GpioPin<portNum, bitPosition>(yanker){
    //todo: coerce making it an input. Not simple as some gpio pins have inverted configuration from the bulk.
  }
  /** use the pin as if it were a boolean variable. */
  inline operator bool() const {
    return GpioPin<portNum, bitPosition>::pin()!=0;
  }
};

/** simple digitial output */
template <PortNumber portNum, BitNumber bitPosition> class OutputPin:public GpioPin<portNum, bitPosition> {
public:
  /** @param yanker controls pullup modality */
  OutputPin(int yanker):GpioPin<portNum, bitPosition>(yanker){
    //todo: coerce making it an input.
  }
  /** use the pin as if it were a boolean variable. */
  inline operator bool() const {
    return GpioPin<portNum, bitPosition>::pin()!=0;
  }

  bool operator =(bool newvalue){
    GpioPin<portNum, bitPosition>::pin()=newvalue?~0:0;//don't need to mask,
    return newvalue;
  }
};


/** simple digital input */
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
    LowActive=0, //level, pulled up
    HighActive, //level, pulled down
    LowEdge,   //edge, pulled up
    HighEdge,  //edge, pulled down
    AnyEdge,   //edge, either edge, input mode buslatch
  };

  /** @param yanker controls pullup modality */
  IrqPin(int yanker):GpioPin<portNum, bitPosition>(yanker){
    //todo: coerce making it an input. Not simple as some gpio pins have inverted configuration from the bulk.
  }
  //todo: functions for dynamic inspection and enabling. //might allow for dynamic redefinition of polarity.
};
}
#endif
