#include "startsignal.h"

#include "nvic.h"

/**
  base  4004 8200 = sysConReg(0x200).

0: "edge control" 0..31 1= rising edge
4: "enable" 0..31 1= enabled
8: "reset" 0..31 1=reset, this is write only.
12: "pending"  1=pending
+16 for 32..39


*/

StartSignal::StartSignal(int which):
  bitnum(which & bitMask(0,5)),
  base(LPC::sysConReg(0x200)+((which&bitMask(5))?16:0))
{
  IrqAccess(which).enable();
}

void StartSignal::configure(bool rising, bool andEnable) const{
  enable(0);//in case we are reconfiguring on the fly
  assignBit(word(0),bitnum,rising);
  prepare();//clear any pending interrupt, especially since configuration might cause a false one.
  if(andEnable){
    enable(1);
  }
}

void StartSignal::enable(bool on) const {
   assignBit(word(4),bitnum,on);
}


