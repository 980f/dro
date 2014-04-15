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


#include "wdt.h"
#include "gpio.h"
#include "syscon.h"

using namespace LPC;

//#define CLOCK_SETUP           1
//#define SYSCLK_SETUP          1
//#define SYSOSC_SETUP          1
//#define SYSOSCCTRL_Val        0x00000000
//#define WDTOSC_SETUP          0
//#define WDTOSCCTRL_Val        0x000000A0
//#define SYSPLLCLKSEL_Val      0x00000001
//#define SYSPLL_SETUP          1
//#define SYSPLLCTRL_Val        0x00000025
//#define MAINCLKSEL_Val        0x00000003

//#define USBCLK_SETUP          1
//#define USBPLL_SETUP          1
//#define USBPLLCLKSEL_Val      0x00000001
//#define USBPLLCTRL_Val        0x00000003
//#define SYSAHBCLKDIV_Val      0x00000001
//#define AHBCLKCTRL_Val        0x0001005F

/*----------------------------------------------------------------------------
 *  Check the register settings
 *----------------------------------------------------------------------------*/
//#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
//#define CHECK_RSVD(val, mask)                     (val & mask)

///* Clock Configuration -------------------------------------------------------*/
//#if (CHECK_RSVD((SYSOSCCTRL_Val), ~0x00000003))
//#error "SYSOSCCTRL: Invalid values of reserved bits!"
//#endif

//#if (CHECK_RSVD((WDTOSCCTRL_Val), ~0x000001FF))
//#error "WDTOSCCTRL: Invalid values of reserved bits!"
//#endif

//#if (CHECK_RANGE((SYSPLLCLKSEL_Val), 0, 2))
//#error "SYSPLLCLKSEL: Value out of range!"
//#endif

//#if (CHECK_RSVD((SYSPLLCTRL_Val), ~0x000001FF))
//#error "SYSPLLCTRL: Invalid values of reserved bits!"
//#endif

//#if (CHECK_RSVD((MAINCLKSEL_Val), ~0x00000003))
//#error "MAINCLKSEL: Invalid values of reserved bits!"
//#endif

//#if (CHECK_RANGE((USBPLLCLKSEL_Val), 0, 1))
//#error "USBPLLCLKSEL: Value out of range!"
//#endif

//#if (CHECK_RSVD((USBPLLCTRL_Val), ~0x000001FF))
//#error "USBPLLCTRL: Invalid values of reserved bits!"
//#endif

//#if (CHECK_RSVD((USBPLLUEN_Val), ~0x00000001))
//#error "USBPLLUEN: Invalid values of reserved bits!"
//#endif

//#if (CHECK_RANGE((SYSAHBCLKDIV_Val), 0, 255))
//#error "SYSAHBCLKDIV: Value out of range!"
//#endif

//#if (CHECK_RSVD((AHBCLKCTRL_Val), ~0x0001FFFF))
//#error "AHBCLKCTRL: Invalid values of reserved bits!"
//#endif

//#if (CHECK_RSVD((SYSMEMREMAP_Val), ~0x00000003))
//#error "SYSMEMREMAP: Invalid values of reserved bits!"
//#endif


/*----------------------------------------------------------------------------
 *  DEFINES
 *----------------------------------------------------------------------------*/


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
  LPC::GPIO::Init(); // needed before static constructors for its defined classes run.


//#if (MEMMAP_SETUP || MEMMAP_INIT)       /* Memory Mapping Setup               */
//  theSYSCON.SYSMEMREMAP = SYSMEMREMAP_Val;
//#endif

} // SystemInit
