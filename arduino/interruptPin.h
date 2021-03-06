#pragma once


typedef void (*Isr)();

#ifdef ARDUINO_MAPLE_RET6
//patch around DUE addition:
constexpr uint8 digitalPinToInterrupt(unsigned arduinoNumber){
  return arduinoNumber;
}
/////**
//// * The kind of transition on an external pin which should trigger an
//// * interrupt.
//// */
////typedef enum ExtIntTriggerMode {
////    RISING, /**< To trigger an interrupt when the pin transitions LOW
////                 to HIGH */
////    FALLING, /**< To trigger an interrupt when the pin transitions
////                  HIGH to LOW */
////    CHANGE /**< To trigger an interrupt when the pin transitions from
////                LOW to HIGH or HIGH to LOW (i.e., when the pin
////                changes). */
////} ExtIntTriggerMode;  c:2, f:3, r:4  ->    c:2  f:1  r:0 ==4-
//
//void attachInterrupt(uint8 pinn, Isr isr, unsigned dueEnum){
//  attachInterrupt(pinn,isr, ExtIntTriggerMode(4-dueEnum));
//}
#endif

/**
Per pin interrupts.

style is one of:
    LOW to trigger the interrupt whenever the pin is low,
    CHANGE to trigger the interrupt whenever the pin changes value
    RISING to trigger when the pin goes from low to high,
    FALLING for when the pin goes from high to low. 
The Due board allows also:
    HIGH to trigger the interrupt whenever the pin is high. 

*/
template <Isr isr, unsigned pinNumber,unsigned style>  struct InterruptPin {

  /** typically invoked in setup(). Invoking the isr once without an interrupt is often useful to deal with an interrupt possibly having occurred while the program was restarting. */
  void attach(bool andInvoke=false)const{
    attachInterrupt(digitalPinToInterrupt(pinNumber), isr, ExtIntTriggerMode(style));
    if(andInvoke){
      isr();
    }
  }

  /** no provision for temporary disabling, so use this then call attach() again later. */
  void detach()const{
    detachInterrupt(digitalPinToInterrupt(pinNumber)); 
  }

  //this always works, howevre most chips have a seperate bit for doing this, so ... we code enable/disable as an assignment to a bit:
  void operator=(bool enable)const{
    if(enable){
      attach(false);
    } else {
      detach();
    }
  }

};

/** abstract base class, overload to add isr rather than pass one in to template, reduces naming efforts.
@deprecated: untested.  This appraich doesn't seem useful for cases where the isr needs access to data outside the object. */
template <unsigned pinNumber,unsigned style>  struct Interruptor {
  virtual void isr(void)=0;

  /** typically invoked in setup(). Invoking the isr once without an interrupt is often useful to deal with an interrupt possibly having occurred while the program was restarting. */
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

  //this always works, however most chips have a seperate bit for doing this, so ... we code enable/disable as an assignment to a bit:
  void operator=(bool enable)const{
    if(enable){
      attach(false);
    } else {
      detach();
    }
  }

};


