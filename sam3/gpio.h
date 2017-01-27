#ifndef GPIO_H
#define GPIO_H

#include "peripheraltypes.h"
#include "feature.h"

/** making out of line instance first, will template later */
class GPIO {
  const unsigned base;
  const unsigned mask;

public:
  /* there are around 14 features, @param which is 0 to 14 */
  const SAM::Feature feature(unsigned which, bool inverted) const;
public:
  GPIO(unsigned portnum, unsigned bitnumber);

  /** @param function is 0 for pin, 1 for A peripheral, 2 for B peripheral,
   * @param extras depends upon @param output:
for input this device supports: pullup, float, but not pulldown
for output this device supports: normal, bussed
*/
  void setMode(unsigned function, bool output, unsigned extras=0);
  void setStyle(IrqStyle irqStyle);
  /** for input you can filter it: none=0, abit=1 (fast clock) , more=2 (per port slow rate) */
  void setGlitcher(unsigned howmuch);
};

/** those settings that are shared across a port */
class PIOPort {
  const unsigned base;
  /** unlike stm the base is not derivable from the pid */
  const unsigned pid;
public:
  /** @param on when true locks out spurious writes to the control register */
  PIOPort(unsigned portnum);
  enum Register {
    GlitchClock,
    ControlLock,
  };
  void enable(bool clockOn)const;
  void reset()const;
  /** @param on when true locks out spurious writes to the control register */
  void configurationLock(bool on)const;
};

#if 0

there is a write protect feature, "PIO "+0 to allow configuration, 1 to disable it.

0x400E0410 clock enable base, /feature.
#endif

#endif // GPIO_H
