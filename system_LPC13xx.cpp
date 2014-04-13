/**************************************************************************//**
* @file     system_LPC13xx.c
* @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Source File
*           for the NXP LPC13xx Device Series
* @version  V1.02
* @date     18. February 2010
*
* @note
* Copyright (C) 2009 ARM Limited. All rights reserved.
*
* @par
* ARM Limited (ARM) is supplying this software for use with Cortex-M
* processor based microcontrollers.  This file can be freely distributed
* within development tools that are supporting such ARM based processors.
*
* @par
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
* heavily modified by alh as true C++ instead of wads of defines.
*
******************************************************************************/

// ******** Code Red **************
// * Changed USBCLK_SETUP to 1
// * Changed SYSPLLCTRL_Val to 0x25
// ********************************

#include <stdint.h>
#include "core_cm3.h"
#include "lpc13xx.h"

#include "wdt.h"
#include "gpio.h"
#include "syscon.h"

using namespace LPC;

#define CLOCK_SETUP           1
#define SYSCLK_SETUP          1
#define SYSOSC_SETUP          1
#define SYSOSCCTRL_Val        0x00000000
#define WDTOSC_SETUP          0
#define WDTOSCCTRL_Val        0x000000A0
#define SYSPLLCLKSEL_Val      0x00000001
#define SYSPLL_SETUP          1
#define SYSPLLCTRL_Val        0x00000025
#define MAINCLKSEL_Val        0x00000003

#define USBCLK_SETUP          1
#define USBPLL_SETUP          1
#define USBPLLCLKSEL_Val      0x00000001
#define USBPLLCTRL_Val        0x00000003
#define SYSAHBCLKDIV_Val      0x00000001
#define AHBCLKCTRL_Val        0x0001005F

/*--------------------- Memory Mapping Configuration -------------------------
 *  //
 *  // <e> Memory Mapping
 *  //   <o1.0..1> System Memory Remap (Register: SYSMEMREMAP)
 *  //                     <0=> Bootloader mapped to address 0
 *  //                     <1=> RAM mapped to address 0
 *  //                     <2=> Flash mapped to address 0
 *  //                     <3=> Flash mapped to address 0
 *  // </e>
 */
#define MEMMAP_SETUP          0
#define SYSMEMREMAP_Val       0x00000001

/*
 *  //-------- <<< end of configuration section >>> ------------------------------
 */

/*----------------------------------------------------------------------------
 *  Check the register settings
 *----------------------------------------------------------------------------*/
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

/* Clock Configuration -------------------------------------------------------*/
#if (CHECK_RSVD((SYSOSCCTRL_Val), ~0x00000003))
#error "SYSOSCCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((WDTOSCCTRL_Val), ~0x000001FF))
#error "WDTOSCCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((SYSPLLCLKSEL_Val), 0, 2))
#error "SYSPLLCLKSEL: Value out of range!"
#endif

#if (CHECK_RSVD((SYSPLLCTRL_Val), ~0x000001FF))
#error "SYSPLLCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((MAINCLKSEL_Val), ~0x00000003))
#error "MAINCLKSEL: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((USBPLLCLKSEL_Val), 0, 1))
#error "USBPLLCLKSEL: Value out of range!"
#endif

#if (CHECK_RSVD((USBPLLCTRL_Val), ~0x000001FF))
#error "USBPLLCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((USBPLLUEN_Val), ~0x00000001))
#error "USBPLLUEN: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((SYSAHBCLKDIV_Val), 0, 255))
#error "SYSAHBCLKDIV: Value out of range!"
#endif

#if (CHECK_RSVD((AHBCLKCTRL_Val), ~0x0001FFFF))
#error "AHBCLKCTRL: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((SYSMEMREMAP_Val), ~0x00000003))
#error "SYSMEMREMAP: Invalid values of reserved bits!"
#endif


/*----------------------------------------------------------------------------
 *  DEFINES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *  Define clocks
 *----------------------------------------------------------------------------*/
#define __XTAL            (12000000UL)    /* Oscillator frequency             */
#define __SYS_OSC_CLK     (__XTAL)   /* Main oscillator frequency        */
#define __IRC_OSC_CLK     (12000000UL)    /* Internal RC oscillator frequency */


#if (CLOCK_SETUP)                         /* Clock Setup              */
#if (SYSCLK_SETUP)                      /* System Clock Setup       */
#if ! (WDTOSC_SETUP)                    /* Watchdog Oscillator Setup*/
#undef WDTOSCCTRL_Val
#define WDTOSCCTRL_Val 5 << 5;
#endif  // WDTOSC_SETUP
#define __WDT_OSC_CLK       wdt_osc_freq(WDTOSCCTRL_Val);

/* sys_pllclkin calculation */
#if   ((SYSPLLCLKSEL_Val & 0x03) == 0)
#define __SYS_PLLCLKIN           (__IRC_OSC_CLK)
#elif ((SYSPLLCLKSEL_Val & 0x03) == 1)
#define __SYS_PLLCLKIN           (__SYS_OSC_CLK)
#elif ((SYSPLLCLKSEL_Val & 0x03) == 2)
#define __SYS_PLLCLKIN           (__WDT_OSC_CLK)
#else
#define __SYS_PLLCLKIN           (0)
#endif

#if (SYSPLL_SETUP)                    /* System PLL Setup         */
#define  __SYS_PLLCLKOUT         (__SYS_PLLCLKIN * ((SYSPLLCTRL_Val & 0x01F) + 1))
#else
#define  __SYS_PLLCLKOUT         (__SYS_PLLCLKIN * (1))
#endif  // SYSPLL_SETUP

/* main clock calculation */
#if   ((MAINCLKSEL_Val & 0x03) == 0)
#define __MAIN_CLOCK             (__IRC_OSC_CLK)
#elif ((MAINCLKSEL_Val & 0x03) == 1)
#define __MAIN_CLOCK             (__SYS_PLLCLKIN)
#elif ((MAINCLKSEL_Val & 0x03) == 2)
#define __MAIN_CLOCK             (__WDT_OSC_CLK)
#elif ((MAINCLKSEL_Val & 0x03) == 3)
#define __MAIN_CLOCK             (__SYS_PLLCLKOUT)
#else
#define __MAIN_CLOCK             (0)
#endif

#define __SYSTEM_CLOCK             (__MAIN_CLOCK / SYSAHBCLKDIV_Val)

#else // SYSCLK_SETUP
#if (SYSAHBCLKDIV_Val == 0)
#define __SYSTEM_CLOCK           (0)
#else
#define __SYSTEM_CLOCK           (__XTAL / SYSAHBCLKDIV_Val)
#endif
#endif // SYSCLK_SETUP

#else // if (CLOCK_SETUP)
#define __SYSTEM_CLOCK               (__XTAL)
#endif  // CLOCK_SETUP


///*----------------------------------------------------------------------------
// *  Clock Variable definitions
// *----------------------------------------------------------------------------*/
// uint32_t SystemCoreClock = __SYSTEM_CLOCK; /*!< System Clock Frequency (Core Clock)*/


/*----------------------------------------------------------------------------
 *  Clock functions
 *----------------------------------------------------------------------------*/
static DefineSingle(SYSCON, apb0Device(18));
unsigned pllInputHz(void){
  switch(theSYSCON.SYSPLLCLKSEL & 0x03) {
  case 0:                       /* Internal RC oscillator             */
    return __IRC_OSC_CLK;

  case 1:                       /* System oscillator                  */
    return __SYS_OSC_CLK;

  case 2:                       /* WDT Oscillator                     */
    return LPC::WDT::osc_hz(theSYSCON.WDTOSCCTRL);
  } // switch
  return 0;
} // pllInputHz

unsigned coreInputHz(){
  switch(theSYSCON.MAINCLKSEL & 0x03) {
  case 0:                             /* Internal RC oscillator             */
    return __IRC_OSC_CLK;

    break;
  case 1:                             /* Input Clock to System PLL          */
    return pllInputHz();

  case 2:                             /* WDT Oscillator                     */
    return LPC::WDT::osc_hz(theSYSCON.WDTOSCCTRL);

    break;
  case 3: /* System PLL Clock Out               */
    if(theSYSCON.SYSPLLCTRL & 0x180) {
      pllInputHz();
    } else {
      return pllInputHz() * 1 + (theSYSCON.SYSPLLCTRL & 0x01F);
    }
    break;
  } // switch
} // coreInputHz

unsigned SystemCoreClockUpdate (void){           /* Get Core Clock Frequency      */
  return coreInputHz() / theSYSCON.SYSAHBCLKDIV;
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void){
  LPC::GPIO::Init(); // needed before static constructors run.

#if (CLOCK_SETUP)                                 /* Clock Setup              */
#if (SYSCLK_SETUP)                                /* System Clock Setup       */
#if (SYSOSC_SETUP)                              /* System Oscillator Setup  */

  theSYSCON.PDRUNCFG &= ~(1 << 5);         /* Power-up System Osc      */
  theSYSCON.SYSOSCCTRL = SYSOSCCTRL_Val;
  spin(200);
  theSYSCON.SYSPLLCLKSEL = SYSPLLCLKSEL_Val;  /* Select PLL Input         */
  theSYSCON.SYSPLLCLKUEN = 0x01;              /* Update Clock Source      */
  theSYSCON.SYSPLLCLKUEN = 0x00;              /* Toggle Update Register   */
  theSYSCON.SYSPLLCLKUEN = 0x01;
  while(! (theSYSCON.SYSPLLCLKUEN & 0x01)) {    /* Wait Until Updated       */
  }
#if (SYSPLL_SETUP)                              /* System PLL Setup         */
  theSYSCON.SYSPLLCTRL = SYSPLLCTRL_Val;
  theSYSCON.PDRUNCFG &= ~(1 << 7);         /* Power-up SYSPLL          */
  while(! (theSYSCON.SYSPLLSTAT & 0x01)) {       /* Wait Until PLL Locked    */
  }
#endif
#endif // if (SYSOSC_SETUP)
#if (WDTOSC_SETUP)                              /* Watchdog Oscillator Setup*/
  theSYSCON.WDTOSCCTRL = WDTOSCCTRL_Val;
  theSYSCON.PDRUNCFG &= ~(1 << 6);         /* Power-up WDT Clock       */
#endif
  theSYSCON.MAINCLKSEL = MAINCLKSEL_Val;    /* Select PLL Clock Output  */
  theSYSCON.MAINCLKUEN = 0x01;              /* Update MCLK Clock Source */
  theSYSCON.MAINCLKUEN = 0x00;              /* Toggle Update Register   */
  theSYSCON.MAINCLKUEN = 0x01;
  while(! (theSYSCON.MAINCLKUEN & 0x01)) {      /* Wait Until Updated       */
  }
#endif // if (SYSCLK_SETUP)

#if (USBCLK_SETUP)                              /* USB Clock Setup          */
  theSYSCON.PDRUNCFG &= ~(1 << 10);        /* Power-up USB PHY         */
#if (USBPLL_SETUP)                              /* USB PLL Setup            */
  theSYSCON.PDRUNCFG &= ~(1 << 8);        /* Power-up USB PLL         */
  theSYSCON.USBPLLCLKSEL = USBPLLCLKSEL_Val;  /* Select PLL Input         */
  theSYSCON.USBPLLCLKUEN = 0x01;              /* Update Clock Source      */
  theSYSCON.USBPLLCLKUEN = 0x00;              /* Toggle Update Register   */
  theSYSCON.USBPLLCLKUEN = 0x01;
  while(! (theSYSCON.USBPLLCLKUEN & 0x01)) {    /* Wait Until Updated       */
  }
  theSYSCON.USBPLLCTRL = USBPLLCTRL_Val;
  while(! (theSYSCON.USBPLLSTAT & 0x01)) {   /* Wait Until PLL Locked    */
  }
  theSYSCON.USBCLKSEL = 0x00;              /* Select USB PLL           */
#else // if (USBPLL_SETUP)
  theSYSCON.USBCLKSEL = 0x01;              /* Select Main Clock        */
#endif // if (USBPLL_SETUP)
#else // if (USBCLK_SETUP)
  theSYSCON.PDRUNCFG |= (1 << 10);        /* Power-down USB PHY       */
  theSYSCON.PDRUNCFG |= (1 << 8);        /* Power-down USB PLL       */
#endif // if (USBCLK_SETUP)

  theSYSCON.SYSAHBCLKDIV = SYSAHBCLKDIV_Val;
  theSYSCON.SYSAHBCLKCTRL = AHBCLKCTRL_Val;
#endif // if (CLOCK_SETUP)


#if (MEMMAP_SETUP || MEMMAP_INIT)       /* Memory Mapping Setup               */
  theSYSCON.SYSMEMREMAP = SYSMEMREMAP_Val;
#endif
} // SystemInit
