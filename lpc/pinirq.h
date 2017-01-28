#ifndef PINIRQ_H
#define PINIRQ_H

#include "startsignal.h"
#include "nvic.h"

template <unsigned pinIndex> class PinIrq {
public:
  StartSignal<pinIndex> starter;
  Irq<pinIndex> nvic;
  /** configure polarity, @param andEnable is whether to also enable locally */
  void configure(bool rising, bool andEnable) const{
    starter.configure(rising,andEnable);
    if(andEnable){
      nvic.enable();
    }
  }

  /** enable pin and at NVIC */
  void enable(bool on=true) const {
    starter.enable(on);
    nvic.enable(on);
  }

  /** reset/acknowledge/set to trigger again, often issued in an isr so keep it clean and fast. */
  inline void prepare() const  {
    starter.prepare();
    nvic.clear();
  }

  /** whether locally pending, also can check NVIC for 'fully enabled and pending' */
  bool pending() const{
    return  starter.pending();
  }

};

#endif // PINIRQ_H
