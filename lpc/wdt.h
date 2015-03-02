#ifndef LPC_WDT_H
#define LPC_WDT_H

#include "peripheraltypes.h"

namespace LPC {
  constexpr uint32_t wdt_osc_table[] = {
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
  };

  class WDT {
private:
    WDT();
public:
    SFR MOD;
    SFR TC;
    SFR FEED;
    const SFR TV;
    SKIPPED RESERVED0;
    SFR WARNINT; /*!< Offset: 0x014 Watchdog timer warning int. register (R/W) */
    SFR WINDOW; /*!< Offset: 0x018 Watchdog timer window value register (R/W) */

    // a few pieces are in the syscon register, manipulators however are coded here:
    /** */
    constexpr static uint32_t numerator(unsigned int sel){
      return wdt_osc_table[(sel >> 5) & 15];
    }

    constexpr static uint32_t denominator(unsigned int sel){
      return 2 * (1 + (sel & 31));
    }

    constexpr static uint32_t osc_hz(unsigned int sel){
      return numerator(sel) / denominator(sel);
    }
  };
} // namespace LPC
#endif // LPC_WDT_H
