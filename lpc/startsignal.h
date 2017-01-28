#pragma once

#include "lpcperipheral.h"
/**
  The GPIO pins have interrupt capability that is shared amongst the pins of the port.
  They also have individual interrupts managed off on the side.
  This other interrupt stuff is called "start logic", as the signals involved also cause exits from powerdown modes.

  The GPIO while not individually vectored has more options as to what constitutes an interrupt, the stuff here is edge-triggered only with configurable polarity.

  An instance of this class manages the interrupt configuration.
  There is a lot of duplication inside the chip between the logic here and in the NVIC. You will have to decide if you need the fancy nested enabling logic of the NVIC stuff,
  else you can just leave that on and manage the interrupt with const instances of this class.

  base  4004 8200 = sysConReg(0x200).

0: "edge control" 0..31 1= rising edge
4: "enable" 0..31 1= enabled
8: "reset" 0..31 1=reset, this is write only.
12: "pending"  1=pending
+16 for 32..39

*/

template <unsigned pinIndex> class StartSignal {
  enum{
    bitnum= pinIndex & bitMask(0,5), //0..31, see gpio for mapping from ports to the value here.
    base=(LPC::sysConBase(0x200)+((pinIndex>>5)*16)), //register base, there are 4 registers per group.
    mask=bitMask(bitnum)
  };
  /** there are 4 groups/attributes, @param group is the byte offset from the lpc manual. */
  unsigned &word(unsigned group) const {
    return *atAddress(base+group);
  }

public:
  /** configure polarity, @param andEnable is whether to also enable locally */
  void configure(bool rising, bool andEnable) const{
    enable(0);//in case we are reconfiguring on the fly
    assignBit(word(0),bitnum,rising);
    prepare();//clear any pending interrupt, especially since configuration might cause a false one.
    if(andEnable){
      enable(1);
    }
  }
  /** enable locally, doesn't alter NVIC state */
  void enable(bool on=true) const {
    assignBit(word(4),bitnum,on);
  }
  /** reset/acknowledge/set to trigger again, often issued in an isr so keep it clean and fast. */
  inline void prepare() const  {
    word(8)=mask;//write a 1 to clear, 0 unchanged
  }
  /** whether locally pending, also can check NVIC for 'fully enabled and pending' */
  bool pending() const{
    return  bit(word(12),bitnum);
  }
};

