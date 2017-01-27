#ifndef GPIO_H
#define GPIO_H

#include "peripheraltypes.h"
#include "feature.h"


enum Port {
  PA=0,
  PB,
  PC
};

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
class PIOPort {
  const unsigned base;
public:
  /** unlike stm the base is not derivable from the pid */
  const unsigned pid;
public:
  /** @param on when true locks out spurious writes to the control register */
  PIOPort(Port portnum);
  enum Register {
    GlitchClock=0x8C,//4th member of feature 8
    ControlLock=14,//at feature 14 address.
  };

  void enable(bool clockOn)const;
  /** @param on when true locks out spurious writes to the control register */
  void configurationLock(bool on)const;
  /* set glitch filter options for the bank, @see also */
  void setGlitchFilterBase(unsigned slowrate)const;
  /** read interrupt flag register, bit per pin, read resets flags so this must be done at group level */
  unsigned getInterrupts()const;
};

/** making out of line instance first, will template later */
class GPIO {
  const unsigned base;
  const unsigned mask;

public:
  /* there are around 14 features, @param which is 0 to 14 */
  const SAM::Feature feature(GpioFeature which, bool inverted) const;
public:
  GPIO(Port portnum, unsigned bitnumber);

  /** @param function is 0 for pin, 1 for A peripheral, 2 for B peripheral,
   * @param extras depends upon @param output:
for input see PinBias
for output this device supports: normal, bussed
*/
  void setMode(unsigned function, bool output, unsigned extras=0);
  /** this sets how the pin affects the grouped interrupt. */
  void setStyle(IrqStyle irqStyle);
  /** for input you can filter it: none=0, abit=1 (fast clock) , more=2 (per port slow rate) */
  void setGlitcher(unsigned howmuch);
};


#if 0

there is a write protect feature, "PIO "+0 to allow configuration, 1 to disable it.
for enable:
0x400E0410 clock enable base, /feature.
for Reset
#endif

#endif // GPIO_H
