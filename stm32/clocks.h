#ifndef CLOCKS_H
#define CLOCKS_H

#include "eztypes.h"
#include "peripheral.h"

extern const u32 EXTERNAL_HERTZ;

//clock rate:
/** bus is: 0:ahb; 1:apb1; 2:apb2; 3:adc; -1:sysclock;*/
u32 clockRate(unsigned int bus);

/**set system clocks to the fastest possible*/
void warp9(bool internal);
/**MCO pin configuration (to snoop on internal clock)*/
void setMCO(unsigned int mode);


#endif // CLOCKS_H
