/* dro project
 * first user of lpcbase so building it in that directory, will later split into project and library.
 */

#include "lpc13xx.h"

#include "gpio.h" //lpcexpresso
#include "nvic.h" //alh

#include "cheapTricks.h"

//p0-4,p05 for qei.
//using defines rather than enum due to limitations of IrqName macro

const PortNumber qeiPort(0);
#define primePhase 4
#define otherPhase 5
#define togglePhase(ph) (primePhase+otherPhase-(ph))

Irq qeiPrimeIrq(4);

int axis;
//prime phase interrupt
void IrqName(4)(void) {//interrupt number and bit number coincidentally the same. Need to add mechanism for this.
  bool dirbit = GPIO::GetValue(qeiPort,togglePhase(primePhase));
  if (dirbit) {
    ++axis; //ignoring quarter phase for now
  } else {
    --axis; //will be +/-4 here
  }
}

//p2-11, p2-10 for index and eot.
/**configuration:
 * polarities for each physical input
 * reverse direction
 */
int wtf(int complaint) {
  static int lastWtf = 0;
  static int repeats = 0;
  if (complaint) {
    if (changed(lastWtf, complaint)) {
      repeats = 0;
    } else {
      ++repeats;
    }
  }
  return complaint;
}

int main(void) {
  //soft stuff
  axis = 0;
  //hw stuff
  GPIO::Init();
  //live with input defaults for now, the lpc does not have a sane memory organization for its configuration controls.
  //interrupt defaults to edge sensitive
  qeiPrimeIrq.enable();

  while (1) {
//    WFE();

  }
  return 0;
}


////////////// cross compilation fakeouts:
//vendor/chip/board specific reset generation, don't want to reset just the core.
extern "C" void generateHardReset(void);
void generateHardReset(void){

}

//vendore specific code must provide a clock source, architecture various greatly across vendors.
u32 clockRate(int which){
  return 12000000;
}
