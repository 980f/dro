#ifndef CLOCKS_H
#define CLOCKS_H



//todo: move the following into a variant.h file, once we start dealing with more than one variant.
#define __IRC_OSC_CLK     (12000000UL)    /* Internal RC oscillator frequency */

/*----------------------------------------------------------------------------
 *  Define clocks
 *----------------------------------------------------------------------------*/
#ifndef __XTAL
#warning __XTAL not defined, setting it same as internal RC just to get code to compile.
#define __XTAL __IRC_OSC_CLK
#endif


//class Clocks
//{
//public:
//  Clocks();
//};

/*----------------------------------------------------------------------------
 *  Processor Clock functions
 *----------------------------------------------------------------------------*/
unsigned pllInputHz(void);
unsigned coreInputHz();
unsigned coreHz(void);

#endif // CLOCKS_H
