#pragma once

#include "lpcperipheral.h"
/**
  The GPIO pins have interrupt capability that is shared amongst the pins of the port.
  They also have individual interrupts managed off on the side.
  This other interrupt stuff is called "start logic", as the signals involved also cause exits from powerdown modes.

  The GPIO while not individually vectored has more potions as to what constitutes an interrupt, the stuff here is edge-triggered only with configurable polarity.

  An instance of this class manages the interrupt configuration.
  There is a lot of duplication inside the chip between the logic here and in the NVIC. You will have to decide if you need the fancy nested enabling logic of the NVIC stuff,
   else you can just leave that on and manage the interrupt with const instances of this class.
*/

class StartSignal {
  const unsigned bitnum; //0..31, see gpio for mapping from ports to the value here.
  const unsigned base; //register base, there are 4 registers per group.
  /** there are 4 groups/attributes, @param group is the byte offset from the lpc manual. */
  unsigned &word(unsigned group) const {
    return atAddress(base+group);
  }

public:
  /** construction also enables the irq in the NVIC.
    if you statically construct one of these you will have to ensure its InitPriority follows NVIC configuration (but I don'tthink there is any ;) */
  StartSignal(int which);
  /** configure polarity, @param andEnable is whether to also enable locally */
  void configure(bool rising, bool andEnable) const;
  /** enable locally, doesn't alter NVIC state */
  void enable(bool on=true) const;
  /** reset/acknowledge/set to trigger again*/
  void prepare() const;
  /** whether locally pending, also can check NVIC for 'fully enabled and pending' */
  bool pending() const;
};

