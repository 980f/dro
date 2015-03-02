#ifndef PWMOUTPUT_H
#define PWMOUTPUT_H

#include "timer.h"

/** high precision (independent timer) pwm */
class PwmOutput {
  CCUnit output; //some of the timer is shared, beware!
public:
  PwmOutput(const Timer &timer, int ccluno);
  void init(double HzOfCycle,unsigned alt);
  double getCycle(void);//return actual HzOfCycle
  /** 0 = output always low, 1.0==output always high
    * do NOT call from ISR
    */
  void setDuty(double fractionofcycle);
  /** do NOT call from ISR */
  double getDuty(void)const;
};

#endif // PWMOUTPUT_H
