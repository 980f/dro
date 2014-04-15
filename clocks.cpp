#include "clocks.h"
#include "syscon.h"
#include "wdt.h"

using namespace LPC;

static DefineSingle(SYSCON, sysConReg(0));

unsigned pllInputHz(void){
  switch(theSYSCON.SYSPLLCLKSEL & 0x03) {
  case 0:                       /* Internal RC oscillator             */
    return __IRC_OSC_CLK;
  case 1:                       /* System oscillator                  */
    return __XTAL;
    //the case 2 was gotten from cmsis code which either is bullshit or undocumented feature of the part
//  case 2:                       /* WDT Oscillator                     */
//    return LPC::WDT::osc_hz(theSYSCON.WDTOSCCTRL);
  } // switch
  return 0;
} // pllInputHz

unsigned coreInputHz(){
  switch(theSYSCON.MAINCLKSEL & 0x03) {
  case 0:                             /* Internal RC oscillator             */
    return __IRC_OSC_CLK;
  case 1:                             /* Input Clock to System PLL          */
    return pllInputHz();
  case 2:                             /* WDT Oscillator                     */
    return LPC::WDT::osc_hz(theSYSCON.WDTOSCCTRL);
  case 3: /* System PLL Clock Out               */
    if(theSYSCON.SYSPLLCTRL & 0x180) {
      return pllInputHz();
    } else {
      return pllInputHz() * 1 + (theSYSCON.SYSPLLCTRL & 0x01F);
    }
  } // switch
  return 0;//hopefully caller pays attention to this bogus value
} // coreInputHz

unsigned coreHz(){
  return coreInputHz() / theSYSCON.SYSAHBCLKDIV;
}


//todo: if XTAL is not zero then turn on hs-external osc and use it.

//  theSYSCON.PDRUNCFG &= ~(1 << 5);         /* Power-up System Osc      */
//  theSYSCON.SYSOSCCTRL = SYSOSCCTRL_Val;
//  spin(200);
//  theSYSCON.SYSPLLCLKSEL = SYSPLLCLKSEL_Val;  /* Select PLL Input         */
//  theSYSCON.SYSPLLCLKUEN = 0x01;              /* Update Clock Source      */
//  theSYSCON.SYSPLLCLKUEN = 0x00;              /* Toggle Update Register   */
//  theSYSCON.SYSPLLCLKUEN = 0x01;
//  while(! (theSYSCON.SYSPLLCLKUEN & 0x01)) {    /* Wait Until Updated       */
//  }
//#if (SYSPLL_SETUP)                              /* System PLL Setup         */
//  theSYSCON.SYSPLLCTRL = SYSPLLCTRL_Val;
//  theSYSCON.PDRUNCFG &= ~(1 << 7);         /* Power-up SYSPLL          */
//  while(! (theSYSCON.SYSPLLSTAT & 0x01)) {       /* Wait Until PLL Locked    */
//  }
//#endif
//#endif // if (SYSOSC_SETUP)
//#if (WDTOSC_SETUP)                              /* Watchdog Oscillator Setup*/
//  theSYSCON.WDTOSCCTRL = WDTOSCCTRL_Val;
//  theSYSCON.PDRUNCFG &= ~(1 << 6);         /* Power-up WDT Clock       */
//#endif
//  theSYSCON.MAINCLKSEL = MAINCLKSEL_Val;    /* Select PLL Clock Output  */
//  theSYSCON.MAINCLKUEN = 0x01;              /* Update MCLK Clock Source */
//  theSYSCON.MAINCLKUEN = 0x00;              /* Toggle Update Register   */
//  theSYSCON.MAINCLKUEN = 0x01;
//  while(! (theSYSCON.MAINCLKUEN & 0x01)) {      /* Wait Until Updated       */
//  }
//#endif // if (SYSCLK_SETUP)

//#if (USBCLK_SETUP)                              /* USB Clock Setup          */
//  theSYSCON.PDRUNCFG &= ~(1 << 10);        /* Power-up USB PHY         */
//#if (USBPLL_SETUP)                              /* USB PLL Setup            */
//  theSYSCON.PDRUNCFG &= ~(1 << 8);        /* Power-up USB PLL         */
//  theSYSCON.USBPLLCLKSEL = USBPLLCLKSEL_Val;  /* Select PLL Input         */
//  theSYSCON.USBPLLCLKUEN = 0x01;              /* Update Clock Source      */
//  theSYSCON.USBPLLCLKUEN = 0x00;              /* Toggle Update Register   */
//  theSYSCON.USBPLLCLKUEN = 0x01;
//  while(! (theSYSCON.USBPLLCLKUEN & 0x01)) {    /* Wait Until Updated       */
//  }
//  theSYSCON.USBPLLCTRL = USBPLLCTRL_Val;
//  while(! (theSYSCON.USBPLLSTAT & 0x01)) {   /* Wait Until PLL Locked    */
//  }
//  theSYSCON.USBCLKSEL = 0x00;              /* Select USB PLL           */
//#else // if (USBPLL_SETUP)
//  theSYSCON.USBCLKSEL = 0x01;              /* Select Main Clock        */
//#endif // if (USBPLL_SETUP)
//#else // if (USBCLK_SETUP)
//  theSYSCON.PDRUNCFG |= (1 << 10);        /* Power-down USB PHY       */
//  theSYSCON.PDRUNCFG |= (1 << 8);        /* Power-down USB PLL       */
//#endif // if (USBCLK_SETUP)

//  theSYSCON.SYSAHBCLKDIV = SYSAHBCLKDIV_Val;
//  theSYSCON.SYSAHBCLKCTRL = AHBCLKCTRL_Val;
//#endif // if (CLOCK_SETUP)

