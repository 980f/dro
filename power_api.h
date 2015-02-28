/****************************************************************************
*   $Id:: power_api.h 6085 2011-01-05 23:55:06Z usb00423                   $
*   Project: NXP LPC13xx software example
*
*   Description:
*     Power API Header File for NXP LPC13xx Device Series
* made pure C++ by andyh.
*
****************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
****************************************************************************/
#ifndef __POWER_API_H__
#define __POWER_API_H__

#define PWRROMD_PRESENT

struct ROM {
#ifdef USBROMD_PRESENT  // if device has USB functions
  const USB *pUSBD;
#else
  const unsigned p_usb_not_usable;
#endif /* USBROMD_PRESENT */

  const unsigned p_clib;
  const unsigned p_cand;


/** rom table entry points to a pair of functions: */
  struct PWRD {
    void (*set_pll)(unsigned int cmd[], unsigned int resp[]);
    void (*set_power)(unsigned int cmd[], unsigned int resp[]);
  };
  const PWRD *pPWRD;

  const unsigned p_dev1;
  const unsigned p_dev2;
  const unsigned p_dev3;
  const unsigned p_dev4;
}
ROM;

// PLL setup related definitions
enum CPU_FREQ_RULE { // main PLL freq must be ...
  EQU = 0,    // equal to the specified
  LTE = 1,    // less than or equal the specified
  GTE = 2,    // greater than or equal the specified
  CLOSE = 3       // as close as possible the specified
};

enum PLL_RETURN {
  CMD_CUCCESS,    // PLL setup successfully found ([sic] on C vs S in the spelling)
  INVALID_FREQ,    // specified freq out of range (either input or output)
  INVALID_MODE,    // invalid mode (see above for valid) specified
  FREQ_NOT_FOUND,    // specified freq not found under specified conditions
  NOT_LOCKED,   // PLL not locked => no changes to the PLL setup
};

// power setup elated definitions
enum POWER_PARAM {
  DEFAULT, // default power settings (voltage regulator, flash interface)
  CPU_PERFORMANCE, // setup for maximum CPU performance (higher current, more computation)
  EFFICIENCY, // balanced setting (power vs CPU performance)
  LOW_CURRENT, // lowest active current, lowest CPU performance
}
// same as PLL return
// #define	PARAM_CMD_CUCCESS		0   //power setting successfully found
// #define	PARAM_INVALID_FREQ		1   //specified freq out of range (=0 or > 50 MHz)
// #define	PARAM_INVALID_MODE		2   //specified mode not valid (see above for valid)

#define MAX_CLOCK_KHZ_PARAM     72000

#endif  /* __POWER_API_H__ */
