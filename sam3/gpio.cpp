#include "gpio.h"

#include "samperipheral.h"

using namespace SAM;

const SAM::Feature GPIO::feature(GpioFeature which,bool inverted)const{
  return SAM::Feature(base+(which<<4),mask,inverted);
}

//GPIO::GPIO(Port portnum, unsigned bitnumber):
//  base(scmbase(portnum+6)),
//  mask(1<<bitnumber){
//  //#nada
//}

//void GPIO::setMode(unsigned function, bool output, unsigned extras){
//  const Feature &justPin=feature(BeSimple,false);

//  if(function==0) {
//    justPin=1;
//    //and periph doesn't matter
//  } else {
//    justPin=0;
//    feature(WhichPeripheral,false)= (function==2);
//  }

//  feature(BeOutput,false)=output;

//  if(output){
//    feature(OpenDrain,false)= extras==1;
//  } else {
//    feature(NoPullup,true)= extras==PullUp;
//  }

//}

//void GPIO::setStyle(IrqStyle irqStyle){
//  switch (irqStyle){

//  }
//}



/** each feature is 16 addresses
0: make pin be a simple pin, else its a peripheral
1: output enable
2: glitch input filter
3: data, set 1's, set 0's, data port is 32 bit writable as well as bit mask readable., 4th group is actual pin level
4: change interrupt, usual triplet, 4th member is 'interrupt latched', clears on read!:)
5: open collector/drain enable
6: *disable* pullup
7: B-peripheral, else A (see also feature 0)
8: glitch filter clock select, on=mck, else div_slck, see feature 2. manual implies opposite polarity on readback. 4th member is slow clock divider.
... Tdiv_slclk = 2*(DIV+1)*Tslow_clock
9: no use
10: lock output state
11: additional interrupt mode(0=both edge per feature 4), no readback
12: edge else level, inverted readback?
13: low edge/low active, inverted readback?
14: isLocked, writeprotect gizmo., writeprotect faulted (clear on read) (no dox on how to map 16 bit code to which register was attacked)



clock bit is 10+portnum, not tied ot address like the nice folks at stm did.



*/
