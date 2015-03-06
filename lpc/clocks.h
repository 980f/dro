#ifndef CLOCKS_H
#define CLOCKS_H


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
