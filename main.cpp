/* dro project
 * first user of lpcbase so building it in that directory, will later split into project and library.
 */

#include "lpc13xx.h"
#include "gpio.h" // lpcexpresso
#include "nvic.h" // alh
#include "core_cmInstr.h" //cm3 intrinsics
#include "cheapTricks.h"
#include "functional"

using namespace LPC;

// p0-4,p05 for qei.

const InputPin<PortNumber(0),BitNumber(4)> primePhase(BusLatch);
const InputPin<0,5> otherPhase(BusLatch);

Irq qeiPrimeIrq(4);

int axis;
// prime phase interrupt
HandleInterrupt(4){ // interrupt number and bit number coincidentally the same. Need to add mechanism for this.
  bool dirbit = otherPhase;

  if(dirbit) {
    ++axis; // ignoring quarter phase for now
  } else {
    --axis; // will be +/-4 here
  }
}

// p2-11, p2-10 for index and eot.
/**configuration:
 * polarities for each physical input
 * reverse direction
 */



class BufferedSender {

};

int wtf(int complaint){
  static int lastWtf = 0;
  static int repeats = 0;

  if(complaint) {
    if(changed(lastWtf, complaint)) {
      repeats = 0;
    } else {
      ++repeats;
    }
  }
  return complaint;
} // wtf

int main(void){
  // soft stuff
  axis = 0;
  // hw stuff

  // live with input defaults for now, the lpc does not have a sane memory organization for its configuration controls.
  // interrupt defaults to edge sensitive
  qeiPrimeIrq.enable();

  while(1) {
    __WFE();
  }
  return 0;
} // main


////////////// cross compilation fakeouts:
// vendor/chip/board specific reset generation, don't want to reset just the core.
extern "C" void generateHardReset(void);
void generateHardReset(void){
}

// vendore specific code must provide a clock source, architecture various greatly across vendors.
u32 clockRate(int which){
  return 12000000;
}

