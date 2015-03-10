#pragma once

#define SYSCON_H

#include "lpcperipheral.h"

/**------------- System Control (SYSCON) --------------------------------------*/

namespace LPC {
  class SYSCON {
    SYSCON();
public:
    SFR SYSMEMREMAP; /* Sys mem. Remap, Offset 0x0   */
    SFR PRESETCTRL;
    SFR SYSPLLCTRL; /* Sys PLL control              */
    SFR SYSPLLSTAT;
    SFR USBPLLCTRL; /* USB PLL control, offset 0x10 */
    SFR USBPLLSTAT;
    SKIPPED RESERVED0[2];

    SFR SYSOSCCTRL; /* Offset 0x20 */
    SFR WDTOSCCTRL;
    SFR IRCCTRL;
    SKIPPED RESERVED1[1];
    SFR SYSRESSTAT; /* Offset 0x30 */
    SKIPPED RESERVED2[3];
    SFR SYSPLLCLKSEL; /* Offset 0x40 */
    SFR SYSPLLCLKUEN;
    SFR USBPLLCLKSEL;
    SFR USBPLLCLKUEN;
    SKIPPED RESERVED3[8];

    SFR MAINCLKSEL; /* Offset 0x70 */
    SFR MAINCLKUEN;
    SFR SYSAHBCLKDIV;
    SKIPPED RESERVED4[1];

    SFR SYSAHBCLKCTRL; /* Offset 0x80 */
    SKIPPED RESERVED5[4];

    SFR SSP0CLKDIV;
    SFR UARTCLKDIV;
    SFR SSP1CLKDIV; /*!< Offset: 0x09C SSP1 clock divider (R/W) */
    SKIPPED RESERVED6[3];
    SFR TRACECLKDIV;

    SFR SYSTICKCLKDIV; /* Offset 0xB0 */
    SKIPPED RESERVED7[3];

    SFR USBCLKSEL; /* Offset 0xC0 */
    SFR USBCLKUEN;
    SFR USBCLKDIV;
    SKIPPED RESERVED8[1];
    SFR WDTCLKSEL; /* Offset 0xD0 */
    SFR WDTCLKUEN;
    SFR WDTCLKDIV;
    SKIPPED RESERVED9[1];
    SFR CLKOUTCLKSEL; /* Offset 0xE0 */
    SFR CLKOUTUEN;
    SFR CLKOUTDIV;
    SKIPPED RESERVED10[5];

    SFR PIOPORCAP0; /* Offset 0x100 */
    SFR PIOPORCAP1;
    SKIPPED RESERVED11[18];

    SFR BODCTRL; /* Offset 0x150 */
    SKIPPED RESERVED12[1];
    SFR SYSTCKCAL;
    SKIPPED RESERVED13[41];

    SFR STARTAPRP0; /* Offset 0x200 */
    SFR STARTERP0;
    SFR STARTRSRP0CLR;
    SFR STARTSRP0;
    SFR STARTAPRP1;
    SFR STARTERP1;
    SFR STARTRSRP1CLR;
    SFR STARTSRP1;
    SKIPPED RESERVED14[4];

    SFR PDSLEEPCFG; /* Offset 0x230 */
    SFR PDAWAKECFG;
    SFR PDRUNCFG;
    SKIPPED RESERVED15[110];
    /** const in the deepest sense, no need for read-only */
    SKIPPED DEVICE_ID;
  };

/** construct one of these to set the selected clock enable */
  template <int bit> struct ClockController: public SFRbit< sysConReg(0x80), bit> {
    ClockController(bool value){
      this->operator =(value);
    }
  };

/** creating one of these resets then conditionally enables a device. */
  template <int bit> struct Resetter: public SFRbit<sysConReg(4), bit> {
    Resetter(bool value = true){
      this->operator =(0);
      this->operator =(value);
    }
  };
} // namespace LPC

