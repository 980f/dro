#ifndef CLOCKS_H
#define CLOCKS_H

#include "target.h"

//LPC common IR frequency
#define LPC_IRC_OSC_CLK     (12000000)    /* Internal RC oscillator frequency */

/*----------------------------------------------------------------------------
 *  Define clocks
 *----------------------------------------------------------------------------*/
#ifndef TARGET_XTAL
#warning "TARGET_XTAL not defined, setting it same as internal RC just to get code to compile."
#define TARGET_XTAL LPC_IRC_OSC_CLK
#endif

/*----------------------------------------------------------------------------
 *  Processor Clock functions
 *----------------------------------------------------------------------------*/
unsigned pllInputHz(void);
unsigned coreInputHz();
unsigned coreHz(void);

///////////////
// portability functions

/** return Hz for @param bus number, -1 for systick, 0 for core, 1.. for AHB etc device specific values usually tied to apb index/*/
unsigned clockRate(int bus);

#endif // CLOCKS_H
