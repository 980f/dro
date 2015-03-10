#ifndef CLOCKS_H
#define CLOCKS_H
#include "eztypes.h"

/** usually defined in a board file: */
extern const u32 EXTERNAL_HERTZ;

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

/** set clock to fastest possible with @internal osc else with its own logic as to which is faster.*/
 void warp9(bool internal);

#endif // CLOCKS_H
