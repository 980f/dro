#ifndef GPIO_H
#define GPIO_H

#include "peripheraltypes.h"
#include "feature.h"
#include "samperipheral.h"

enum Port {
  PA=0,
  PB,
  PC
};

constexpr unsigned portBase(unsigned portnum){
  return scmbase(portnum+6);
}

/** declared outside of xxPin class so that we don't have to apply template args to each use.*/
enum PinBias { //#ordered for MODE field of iocon register
  LeaveFloating = 0, // in case someone forgets to explicitly select a mode
  PullUp, // level, pulled up
};

enum GpioFeature {
  BeSimple=0,
  BeOutput,
  Glitcher,
  Data, // set 1's, set 0's, data port is 32 bit writable as well as bit mask readable., 4th group is actual pin level
  DeltaInterrupt,//4: change interrupt, usual triplet, 4th member is 'interrupt latched', clears on read!:)
  OpenDrain, //5: open collector/drain enable
  NoPullup, //6: *disable* pullup
  WhichPeripheral, //7: B-peripheral, else A (see also feature 0)
  GlitchRate, //8: glitch filter clock select, on=mck, else div_slck, see feature 2. manual implies opposite polarity on readback. 4th member is slow clock divider.
  //... Tdiv_slclk = 2*(DIV+1)*Tslow_clock
  NothingAt9, //9: no use
  LockOutputState, //10: lock output state
  PolarInterrupt, //11: additional interrupt mode(0=both edge per feature 4), no readback
  EdgyInterrupt, //12: edge else level, inverted readback?
  InterruptPolarity, //13: low edge/low active, inverted readback?
  ControlLock, //14: isLocked, writeprotect gizmo., writeprotect faulted (clear on read) (no dox on how to map 16 bit code to which register was attacked)
};


/** those settings that are shared across a port */
template <unsigned portnum> class PIOPort {
  enum {
    base=portBase(portnum), /** unlike stm the base is not derivable from the pid */
    pid=10+portnum,
  };
public:

  void enable(bool clockOn)const;
  /** @param on when true locks out spurious writes to the control register */
  void configurationLock(bool on)const;
  /* set glitch filter options for the bank, @see also */
  void setGlitchFilterBase(unsigned slowrate)const;
  /** read interrupt flag register, bit per pin, read resets flags so this must be done at group level */
  unsigned getInterrupts()const;
};

///** making out of line instance first, will template later */
//class GPIO {
//  const unsigned base;
//  const unsigned mask;

//public:
//  /* there are around 14 features, @param which is 0 to 14 */
//  const SAM::Feature feature(GpioFeature which, bool inverted) const;
//public:
//  GPIO(Port portnum, unsigned bitnumber);
//  //do NOT add a destructor, we don't need to destruct pins.

//  /** @param function is 0 for pin, 1 for A peripheral, 2 for B peripheral,
//   * @param extras depends upon @param output:
//for input see PinBias
//for output this device supports: normal, bussed
//*/
//  void setMode(unsigned function, bool output, unsigned extras=0);
//  /** this sets how the pin affects the grouped interrupt. */
//  void setStyle(IrqStyle irqStyle);
//  /** for input you can filter it: none=0, abit=1 (fast clock) , more=2 (per port slow rate) */
//  void setGlitcher(unsigned howmuch);
//};

/** to configure a pin for a dedicated function one must merely construct a GpioPin with template args for which pin and constructor arg of control pattern*/
template <unsigned portNum, unsigned bitPosition> class PortPin: public BoolishRef {

protected: // for simple gpio you must use an extended class that defines read vs read-write capability.
  enum {
    mask = 1 << bitPosition, // used for port control register access
    base = portBase(portNum), // base for port control
  };

  const SAM::Feature feature(GpioFeature which,bool inverted)const{
    return SAM::Feature(base+(which<<4),mask,inverted);
  }


  inline void setIocon(unsigned pattern)const{

  }

//  /** @returns reference to the masked access port of the register, mask set to the one bit for this pin. @see InputPin and @see OutputPin classes for use, unlike stm32 bitbanding some shifting is still needed. */
//  inline unsigned &pin() const {
//    return *reinterpret_cast<unsigned *>(/*PortPin<portNum, bitPosition>::*/pinn);
//  }

  inline void setRegister(unsigned which)const{
    SAM::Feature arf=feature(which);
    arf=1;
  }

  inline void clearRegister(unsigned which)const{
    SAM::Feature arf=feature(which);
    arf=0;
  }

  inline void assignRegister(unsigned which,bool level)const{
    SAM::Feature arf=feature(which);
    arf=level;
  }

  void setDirection(bool asOutput)const{
    assignRegister(BeOutput,asOutput);
  }

public:

//  /** only special pins should use this directly. */
//  inline PortPin(unsigned pattern){
//    setIocon(pattern);
//  }


  /** read the pin as if it were a boolean variable. */
  inline operator bool() const {
    SAM::Feature arf=feature(Data); // need to check assembler, a shift might be better.
    return arf;
  }

};


/** simple digital input */
template <unsigned portNum, unsigned bitPosition> class InputPin: public PortPin<portNum, bitPosition> {
  using PortPin<portNum, bitPosition>::clearRegister;
  using PortPin<portNum, bitPosition>::setRegister;
  using PortPin<portNum, bitPosition>::assignRegister;

private:
  bool operator =(bool)const {return false;} // private because this is a read-only entity.

public:
  /** @param yanker controls pullup modality */
  InputPin(PinBias yanker = PullUp): PortPin<portNum, bitPosition>(this->ioconPattern(yanker)){
    //todo: set direction to 0, which is the power up setting so not urgent in our typical use of static configuration.
    PortPin<portNum, bitPosition>::setDirection(0);
  }

  void irqAcknowledge() const {
    setRegister();
  }

  void irq(bool enable)const{
    assignRegister(16,enable);
  }

  void setIrqStyle(IrqStyle style, bool andEnable)const{
    switch(style){
    NotAnInterrupt = 0, // in case someone forgets to explicitly select a mode
    AnyEdge, // edge, either edge, input mode buslatch
    LowActive, // level, pulled up
    HighActive, // level, pulled down
    LowEdge, // edge, pulled up
    HighEdge   // edge, pulled down

DeltaInterrupt;
    PolarInterrupt; //11: additional interrupt mode(0=both edge per feature 4), no readback
    EdgyInterrupt; //12: edge else level, inverted readback?
    InterruptPolarity; //13: low edge/low active, inverted readback?

    }
    if(andEnable){
      irq(1);
    }
  }

};

/** simple digital output */
template <unsigned portNum, unsigned bitPosition> class OutputPin: public PortPin<portNum, bitPosition>{
  typedef  PortPin<portNum, bitPosition> super;
public:
  /** @param yanker controls pull-up modality */
  OutputPin(PinBias yanker = BusLatch): PortPin<portNum, bitPosition>(this->ioconPattern(yanker)){
    super::setDirection(1);
  }

  bool operator =(bool newvalue)const{
    super::pin() = newvalue ? ~0 : 0; // don't need to mask or shift, just present all ones or all zeroes and let the hardware 'mask with address' take care of business.
    return newvalue;
  }

};

/** Multiple contiguous bits in a register.
 * This class expedites access using the gpio port 'masked[]' based access
 */
template <PortNumber portNum, unsigned msb, unsigned lsb> class PortField {
  enum {
    // read the lpc manual, certain address bits are used as a mask
    address = portBase(portNum) | bitMask(lsb+2,msb-lsb+1)
  };

public:
  // read
  inline operator unsigned() const {
    return *atAddress(address) >> lsb;
  }
  // write
  inline void operator =(unsigned value) const {
    *atAddress(address) = value << lsb;
  }
  void configurePins(unsigned pattern){
    unsigned pini= pinIndex(portNum, lsb);
    for(unsigned which=lsb;which++<=msb;){
      GPIO::setIocon(pini++,pattern);
    }
  }

};


#if 0

there is a write protect feature, "PIO "+0 to allow configuration, 1 to disable it.

#endif

#endif // GPIO_H
