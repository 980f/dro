#pragma once

typedef void (*Isr)();

/**
Somewhat trivial class to let you declare your isr where convenient, then 


style is one of:
    LOW to trigger the interrupt whenever the pin is low,
    CHANGE to trigger the interrupt whenever the pin changes value
    RISING to trigger when the pin goes from low to high,
    FALLING for when the pin goes from high to low. 
The Due board allows also:
    HIGH to trigger the interrupt whenever the pin is high. 

*/
template <Isr isr, unsigned pinNumber,unsigned style>  struct InterruptPin {

  /** typically invoked in setup() */
  void attach(bool andInvoke=false)const{
    attachInterrupt(digitalPinToInterrupt(pinNumber), isr, style);
    if(andInvoke){
      isr();
    }
  }

  /** no provision for temporary disabling, so use this then call attach() again later. */
  void detach()const{
    detachInterrupt(digitalPinToInterrupt(pinNumber)); 
  }

};


