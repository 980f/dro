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
  u32 controlreg=sysConReg(forUsb?0x10:8);
  mergeBits(controlreg,mpy-1,0,5);
  mergeBits(controlreg,exponent,5,2);
}

unsigned pllInputHz(bool forUsb){
  switch(sysConReg(forUsb?0x48:0x40) & 0x03) {
  case IRCosc:                       /* Internal RC oscillator             */
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
  unsigned mainclksel=atAddress(sysConReg(0x70));
  return coreInputHz(ClockSource(mainclksel & 0x03));
}

unsigned coreHz(){
  return rate(coreInputHz() , atAddress(sysConReg(0x78)));
}

void setMainClockSource(ClockSource cksource){
  //todo:1 check that source is operating!
  atAddress(sysConReg(0x70))=cksource;
  raiseBit(atAddress(sysConReg(0x74)),0);
  //cmsis code (not net the manual) suggests that we can monitor the above bit to see when the new value has taken effect, but why wait? it is at most somethign like 2 of the present clocks and one should be changing the main clock well in advance of doing application timing.
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
    return rate(coreInputHz(),atAddress(sysConReg(0x0B0)));
  case 0: //processor clock
    return coreHz();
    //  case 1:
    //    return 0;
  }
  return LPC_IRC_OSC_CLK;
}


bool switchToPll(unsigned mpy, unsigned exponent){
  clearBit(atAddress(sysConReg(0x238)),7);// pllPowerdown
  setPLL(0,mpy, exponent);
  //wait for lock, //100uS absolute limit
  for(unsigned trials=120;trials-->0;){
    if(bit(atAddress(sysConReg(0x00c)),0)){//if locked
      setMainClockSource(ClockSource::PLLout);
      return true;
    }
  }
  return false;
}


void setMainClock(unsigned hz,ClockSource cksource){
  //divider:
  //
}

void warp9(bool internal){
  u32 sourceHz(0);
  if(internal || EXTERNAL_HERTZ==0 ){
    clearBitAt(sysConReg(0x238),0);//irc power on
    sourceHz=LPC_IRC_OSC_CLK;
  } else {
    clearBitAt(sysConReg(0x238),5);//ext osc on, presumes xtal configged, else we should leave it off and test whether an external clock is actually clicking
    sourceHz=EXTERNAL_HERTZ;
  }
  unsigned exponent=3;//max exponent for divisor
  unsigned scaler=muldivide(MaxFrequency,1<<exponent,sourceHz);//maximum multipler

  while(scaler>32 || scaler*sourceHz>=320000000){//field limit, cco limit
    scaler>>=1;
    if(--exponent==0){
      break;
    }
  }
  //todo: if still above 320MHz then system is not reliable.
  while(isEven(scaler) && scaler*sourceHz>=156000000){//reduce freq if we can without going too low
    scaler>>=1;
    if(--exponent==0){
      break;
    }
  }

  if(scaler){//if we have a solution apply it
    switchToPll(scaler,exponent);
  }
}

void setMCO(ClockSource which, unsigned divider){
  if(divider){
    atAddress(sysConReg(0x0E0))=which;
    atAddress(sysConReg(0x0E8))=divider;
    raiseBit(atAddress(sysConReg(0xE4)),0);
    //and direct it to a pin
    atAddress(ioConReg(0x10))=1; //the values for iocon's defy systemization.
  }
}
