#ifndef nvicH
#define nvicH
#include "stm32.h"

#define resolve(arf) arf

//macro's for generating numbers don't work in the irqnumber slot below. Time to move on ...
#define IrqName(irqnumber) IRQ ## irqnumber

//object based interrupt handlers can simply jump to the handler code once 'this' has been init.
#define ObjectInterrupt(objCall, irqnumber) void IrqName(irqnumber) (void) { objCall; }

#define FaultName(faultIndex) FAULT ## faultIndex
#define FaultHandler(name, faultIndex) void name(void) __attribute__((alias("FAULT" # faultIndex)))

#define HandleFault(faultIndex) void FaultName(faultIndex) (void)


//the nvic is not bitbanded, these macro's access the bit associated with the Irq number.
#define lvalue(grup) reinterpret_cast <int *> (0xE000E000 + grup + (4 * (number >> 5)))
//this is for the registers where you write a 1 to a bit to make something happen.
#define strobe(grup) * lvalue(grup) = 1 << (number & 0x1F)
#define irqflag(grup) (1 & ((*lvalue(grup)) >> (number & 0x1F)))

struct Irq {
  u8 number;

  Irq(int number): number(number), locker(0){}
  /** @return previous setting while inserting new one*/
  u8 setPriority(u8 newvalue) const { //one byte each, often only some high bits are implemented
    u8 *pointer = reinterpret_cast <u8 *> (0xE000E400);
    u8 oldvalue = *pointer;

    *pointer = newvalue;
    return oldvalue;
  }

  bool isActive(void) const {
    return irqflag(0x300);
  }

  bool isEnabled(void) const {
    return irqflag(0x100);
  }

  int locker; //tracking nested attempts to lock out the interrupt.

  void enable(void){
    if(locker > 0) { //if locked then reduce the lock such that the unlock will cause an enable
      --locker;  //one level earlier than it would have. This might be surprising so an
      //unmatched unlock might be the best enable.
    }
    if(locker == 0) { //if not locked then actually enable
      strobe(0x100);
    }
  }

  void disable(void) const {
    strobe(0x180);
  }

  void lock(void){
    if(locker++ == 0) {
      disable();
    }
  }

  void fake(void) const {
    strobe(0x200);
  }

  void clear(void) const {
    strobe(0x280);
  }

  /** clear any pending then enable, regular enable will cause an interrupt if one is pending.*/
  void prepare(void){
    clear();
    enable();
  }

  void uncrustWierdness(void){} //just in this file uncrust.exe was improperly formatting the last function.
};

/** disable interrupt on creation of object, enable it on destruction
  * usage: create one within a block where the irq must not be honored.
  * note: this cheap implementation may turn on an interrupt that was off,
  *  don't lock if you can't tolerate the interrupt being enabled.
  *  Since each interrupt can be stifled at its source this should not be a problem.
  *  future: automate detection of being in the irq service and drop the argument.
  */
struct IRQLock {
  Irq&irq;
public:
  IRQLock(Irq&irq, bool inIrq = false): irq(irq){
    if(!inIrq) {
      irq.lock();
    }
  }
  ~IRQLock(){
    irq.enable();
  }
};

/** e.g. SysTick to lowest: setFaultHandlerPriority(15,255);*/
void setFaultHandlerPriority(int faultIndex, u8 level);
/** value to put into PRIGROUP field, see arm tech ref manual.
  * 0: maximum nesting; 7: totally flat; 2<sup>7-code</sup> is number of different levels
  * cstm32 only implements the 4 msbs of the logic so values 3,2,1 are same as 0*/
void configurePriorityGrouping(int code); //cortexm3.s or stm32.cpp

#ifdef ALH_SIM //just compiling for syntax checking
#define EnableInterrupts
#define DisableInterrupts
#define LOCK(somename)
#define IRQLOCK(irq)
class CriticalSection { //#this stub needed as we don't want to bother #ifdef around the init of the nesting field of the useful instantiaion.
  static volatile int nesting;
};
#else
#define EnableInterrupts asm ("  CPSIE f")
#define DisableInterrupts asm ("  CPSID f")

/** creating one of these in a function (or blockscope) disables interrupts until said function (or blockscope) exits.
  * By using this fanciness you can't accidentally leave interrupts disabled. */
class CriticalSection {
  static volatile int nesting;
public:
  CriticalSection(void){
    DisableInterrupts;
    ++nesting;
  }

  ~CriticalSection (void){
    if(nesting != 0) { //then interrupts are globally disabled
      if(--nesting == 0) {
        EnableInterrupts;
      }
    }
  }
};

//this does not allow for static locking, only for within a function's execution (which is a good thing!):
#define LOCK(somename) CriticalSection somename ## _locker
//this can only handle simple access to the irq, no this-> or submembers:
#define IRQLOCK(irqVarb) IRQLock IRQLCK ## irqVarb(irqVarb)
#endif /* ifdef ALH_SIM */

#undef strobe
#undef lvalue
#endif /* ifndef nvicH */
