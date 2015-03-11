#include "clocks.h"

#include "syscon.h"
#include "wdt.h"
#include "bitbanger.h"
#include "minimath.h" //multiply divide

//LPC common IR frequency
const u32 LPC_IRC_OSC_CLK     (12000000);
//family limit
const u32 MaxFrequency (72000000);

enum ClockSource {
  IRCosc=0,
  PLLin,
  WDTosc,
  PLLout
};

/*----------------------------------------------------------------------------
 *  Define clocks
 *----------------------------------------------------------------------------*/

using namespace LPC;

//static DefineSingle(SYSCON, sysConReg(0));

unsigned pllOutput(bool forUsb){
  u32 controlreg=atAddress(sysConReg(forUsb?0x10:8));
  u32 multiplier=extractField(controlreg,4,0);
  u32 divider=extractField(controlreg,6,5);

  return muldivide( pllInputHz(forUsb) , 1 +multiplier , (1<<divider)) ;
}

void setPLL(bool forUsb,unsigned mpy, unsigned exponent){
  u32 controlreg=sysConReg(forUSb?0x10:8);
  SFRfield<controlreg,0,5> multiplier;
  SFRfield<controlreg,5,2> divider;
  multiplier=mpy-1;
  divider= exponent;
}

unsigned pllInputHz(bool forUsb){
  switch(sysConReg(forUsb?0x48:0x40) & 0x03) {
  case 0:                       /* Internal RC oscillator             */
    return LPC_IRC_OSC_CLK;

  case 1:                       /* System oscillator                  */
    return EXTERNAL_HERTZ;
    // the case 2 was gotten from cmsis code which either is bullshit or undocumented feature of the part
//  case 2:                       /* WDT Oscillator                     */
//    return LPC::WDT::osc_hz(theSYSCON.WDTOSCCTRL);
  } // switch
  return 0;
} // pllInputHz

unsigned coreInputHz(ClockSource cksource){
  switch(cksource){
  case IRCosc:
    return LPC_IRC_OSC_CLK;
  case PLLin:
    return pllInputHz(0);
  case WDTosc:
    return WDT::osc_hz();
  case PLLout:
    return pllOutput(0);
  } // switch
  return 0; // hopefully caller pays attention to this bogus value
} // coreInputHz


unsigned coreInputHz(){
  return coreInputHz(ClockSource(sysConReg(0x70) & 0x03));
}

unsigned coreHz(){
  return rate(coreInputHz() / sysConReg(0x78));
}

void setMainClockSource(ClockSource cksource){
  //todo:1 check that source is operating!
  sysConReg(0x70)=cksource;
  raiseBit(sysConReg(0x74),0);
}

void setMainClock(unsigned hz,ClockSource cksource){
  //divider:

}

// todo: if XTAL is not zero then turn on hs-external osc and use it.

//  theSYSCON.PDRUNCFG &= ~(1 << 5);         /* Power-up System Osc      */
//  theSYSCON.SYSOSCCTRL = SYSOSCCTRL_Val;
//  spin(200);
//  theSYSCON.SYSPLLCLKSEL = SYSPLLCLKSEL_Val;  /* Select PLL Input         */
//  theSYSCON.SYSPLLCLKUEN = 0x01;              /* Update Clock Source      */
//  theSYSCON.SYSPLLCLKUEN = 0x00;              /* Toggle Update Register   */
//  theSYSCON.SYSPLLCLKUEN = 0x01;
//  while(! (theSYSCON.SYSPLLCLKUEN & 0x01)) {    /* Wait Until Updated       */
//  }
// #if (SYSPLL_SETUP)                              /* System PLL Setup         */
//  theSYSCON.SYSPLLCTRL = SYSPLLCTRL_Val;
//  theSYSCON.PDRUNCFG &= ~(1 << 7);         /* Power-up SYSPLL          */
//  while(! (theSYSCON.SYSPLLSTAT & 0x01)) {       /* Wait Until PLL Locked    */
//  }
// #endif
// #endif // if (SYSOSC_SETUP)
// #if (WDTOSC_SETUP)                              /* Watchdog Oscillator Setup*/
//  theSYSCON.WDTOSCCTRL = WDTOSCCTRL_Val;
//  theSYSCON.PDRUNCFG &= ~(1 << 6);         /* Power-up WDT Clock       */
// #endif
//  theSYSCON.MAINCLKSEL = MAINCLKSEL_Val;    /* Select PLL Clock Output  */
//  theSYSCON.MAINCLKUEN = 0x01;              /* Update MCLK Clock Source */
//  theSYSCON.MAINCLKUEN = 0x00;              /* Toggle Update Register   */
//  theSYSCON.MAINCLKUEN = 0x01;
//  while(! (theSYSCON.MAINCLKUEN & 0x01)) {      /* Wait Until Updated       */
//  }
// #endif // if (SYSCLK_SETUP)

// #if (USBCLK_SETUP)                              /* USB Clock Setup          */
//  theSYSCON.PDRUNCFG &= ~(1 << 10);        /* Power-up USB PHY         */
// #if (USBPLL_SETUP)                              /* USB PLL Setup            */
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
// #else // if (USBPLL_SETUP)
//  theSYSCON.USBCLKSEL = 0x01;              /* Select Main Clock        */
// #endif // if (USBPLL_SETUP)
// #else // if (USBCLK_SETUP)
//  theSYSCON.PDRUNCFG |= (1 << 10);        /* Power-down USB PHY       */
//  theSYSCON.PDRUNCFG |= (1 << 8);        /* Power-down USB PLL       */
// #endif // if (USBCLK_SETUP)

//  theSYSCON.SYSAHBCLKDIV = SYSAHBCLKDIV_Val;
//  theSYSCON.SYSAHBCLKCTRL = AHBCLKCTRL_Val;
// #endif // if (CLOCK_SETUP)


unsigned clockRate(int which){
  switch (which) {
  case -1://systick
    return coreInputHz()/theSYSCON.SYSTICKCLKDIV;//todo:0 deal with divide by zero if not enabled.
  case 0: //processor clock
    return coreHz();
//  case 1:
//    return 0;
  }
  return LPC_IRC_OSC_CLK;
}


void warp9(bool internal){
 //todo: RTFM

  if(internal){
    //net product 6:, 6/1, 12/2, 24/4, but not 48/8, max num is 32
  } else {

    unsigned scaler=rate(MaxFrequency,EXTERNAL_HERTZ);

  }
}
