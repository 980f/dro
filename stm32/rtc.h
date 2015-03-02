#ifndef RTC_H
#define RTC_H

#include "stm32.h"

/**
  * 16 bit device!
  * even the 32bit clock  is two separate 16 bit reads.
  * The readable registers are updated on each RTC tick, every 30 microseconds at 32kHz.
  * To get a coherent read one must clear the RSF bit then wait for it to be set, at most
  * one RTC tick later.
  * It is safest to do that once at system init then use periodic interrupts or poll the alarm flag
  * to get updates rather than periodic reads.
  * So: after initializing
  *
  */



class RTC : APBdevice {
  u32 utcSeconds;
  u32 *band; //cached for isr
  u16 *regPointer(int which);
  u32 reg(int which);
  /** only call this after you have set the thingee into configure mode:*/
  void setreg(int which, u32 value);
public:
  void isr(void); //public required for linkage.
public:
  RTC(): APBdevice(1, 10){
    //statically constructed so don't do anything important here
    utcSeconds = 0xFFFFFFFF; //a grossly future value.
    band = getBand();
  }
  /** only call to setup RTC operation*/
  void configure(u32 crystalHz = 32768);
  /** called each boot to enable access to clock value*/
  void init(void);
  /** whether we have been up long enough to read clock, RSF bit*/
  bool okToRead(void);
  u32 getSeconds(void){
    return utcSeconds;
  }
  void setSeconds(u32 seconds);
};

soliton(RTC, APB_Block(0, 10));

#endif // RTC_H
