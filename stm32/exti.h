#ifndef EXTI_H
#define EXTI_H

#include "stm32.h"
/**
stm32F's wildly annoying interrupt input manager

inputs 0:4 have individual interripts, 6+#.
inputs 5..9 share one at 23,
inputs 10..15 share another at 40.

If bit n of one port is watched then no other ports' bit n can be watched.

*/

/* refs gpio port so that enable an interrupt reconfigures the pin to be an input */
#include "gpio.h"
#include "nvic.h" //return irq reference
class Exti :public APBdevice {
  static Irq irqsome[];
  static unsigned irqIndex(unsigned pinnumber);

public:
  Exti();

  /** @returns a reference to the sharred interrupt control object for the pin.
you can set both rising and falling, then check the level directly to figure out which happened
*/
  static Irq &enablePin(const Pin &pin, bool rising, bool falling);
  static void clearPending(const Pin &pin);
};


#endif // EXTI_H
