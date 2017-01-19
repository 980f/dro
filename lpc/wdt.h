#ifndef LPC_WDT_H
#define LPC_WDT_H

#include "lpcperipheral.h"

namespace LPC {
constexpr unsigned wdt_osc_table[] = {
  /* from cmsis code
      400000,
      500000,
      800000,
      1100000,
      1400000,
      1600000,
      1800000,
      2000000,
      2200000,
      2400000,
      2600000,
      2700000,
      2900000,
      3100000,
      3200000,
      3400000
        */

  /* from manual, which also states that the range is 500 kHz through 3.4 MHz which doesn't agree with this table */
  0,
  600000,
  1050000,
  1400000,
  1750000,
  2100000,
  2400000,
  2700000,
  3000000,
  3250000,
  3500000,
  4200000,
  4400000,
  4600000,

};


struct WDT {//could be an inner namespace
  //private:
  //    WDT();
  //public:
  //    SFR MOD;
  //    SFR TC;
  //    SFR FEED;
  //    const SFR TV;
  //    SKIPPED RESERVED0;
  //    SFR WARNINT; /*!< Offset: 0x014 Watchdog timer warning int. register (R/W) */
  //    SFR WINDOW; /*!< Offset: 0x018 Watchdog timer window value register (R/W) */

  // a few pieces are in the syscon register, manipulators however are coded here:
  /** */
  constexpr static unsigned numerator(unsigned sel){
    return wdt_osc_table[ extractBits(sel, 5, 4)];
  }

  constexpr static unsigned denominator(unsigned sel){
    return 1<< (1 + extractBits(sel ,0,5));
  }

  static unsigned osc_hz(){
    unsigned sel=*sysConReg(0x24);
    return numerator(sel) / denominator(sel);
  }

  void static setHertz(unsigned hz){
    //seek best fit
  }

};
} // namespace LPC
#endif // LPC_WDT_H
