#include "pwmoutput.h"
PwmOutput::PwmOutput(const Timer &timer, int ccluno): output(timer, ccluno){}
void PwmOutput::init(double HzOfCycle,unsigned alt){
  //max resolution is at baserate/65536 ~ 1kHz.
  //if rate is slower than that then prescaler will have to be invoked.
  /** in descending order (just like the annoying stm32 manual)
    * __0: don't clear on external trigger.
    * 111: pwm mode 2 (is on for latter part of cycle)
    * __0: don't care if changes are sloppy,
    * __0: not using input trigger,
    * _00: output mode*/
  output.setmode(0b01110000);
  output.takePin(alt,1); //1:what PCU lamp needed.
  //setbase and divider
  output.timer.setCycler(output.timer.ticksForHz(HzOfCycle));
  output.timer.b->enabled=1;//start timebase
}

/** 0 = output always low, 1.0==output always high
  * do NOT call from ISR
  */
void PwmOutput::setDuty(double fractionofcycle){
  unsigned fullOn = output.timer.getCycler(); //ARR
  output.setTicks(saturated(fullOn, fractionofcycle));
}

/** do NOT call from ISR */
double PwmOutput::getDuty(void)const{
  u16 ticks = output.getTicks();
  unsigned fullOn = output.timer.getCycler(); //ARR
  return ratio(double(ticks), double(fullOn)); //ARR
}

double PwmOutput::getCycle(){
  return output.timer.getHz();
}
