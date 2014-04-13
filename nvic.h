#ifndef nvicH
#define nvicH
#include "eztypes.h"
#include "cheapTricks.h"

// internal use, should have been able to pass expressions to IrqName and use 'resolve' to paste the numerical value, but gave up on that working.
#define resolve(arf) arf

/**Internal use. macro's for generating numbers don't work in the irqnumber slot below. Time to move on ...*/
#define IrqName(irqnumber) IRQ ## irqnumber

#define HandleInterrupt(irqnumber) void IrqName(irqnumber) (void)

/** object based interrupt handlers (member function) can simply jump to the handler code once 'this' has been init.
 * this macro creates an interrupt handler that calls a member function of an object */
#define ObjectInterrupt(objCall, irqnumber) void IrqName(irqnumber) (void) { objCall; }

// internal use macro
#define FaultName(faultIndex) FAULT ## faultIndex
// internal use, to build vector table.
#define FaultHandler(name, faultIndex) void name(void) __attribute__((alias("FAULT" # faultIndex)))

/** use this macro to declare that your code that follows it handles the given fault */
#define HandleFault(faultIndex) void FaultName(faultIndex) (void)

// the following macros (using grup) are internal to this module.
// the nvic is not bitbanded, these macro's access the bit associated with the Irq number.
#define lvalue(grup) reinterpret_cast<int *>(0xE000E000 + grup + (4 * (number >> 5)))
// this is for the registers where you write a 1 to a bit to make something happen.
#define strobe(grup) *lvalue(grup) = 1 << (number & 0x1F)
// the interrupt is active
#define irqflag(grup) (1 & ((*lvalue(grup)) >> (number & 0x1F)))

/** struct that will typically results in inline generated calls for manipulating aspects of an nvic interrupt request.
 * Note that interrupt sources may additionally need manipulation, this only covers the nvic part of an interrupt. */
struct Irq {
  /** which request. using an explicit member instead of a template to avert having to code a base class. */
  const int number;
  /** used to manage nested disable/enable */
  int locker; // tracking nested attempts to lock out the interrupt.

  Irq(int number): number(number), locker(0){
    // empty.
  }
  /** @return previous setting while inserting new one*/
  u8 setPriority(u8 newvalue) const { // one byte each, often only some high bits are implemented
    return postAssign(*reinterpret_cast<u8 *>(0xE000E400), newvalue);
  }

  bool isActive(void) const {
    return irqflag(0x300);
  }

  bool isEnabled(void) const {
    return irqflag(0x100);
  }

  /** unlock and if fully unlocked then actually enable it. */
  void enable(void){
    if(locker > 0) { // if locked then reduce the lock such that the unlock will cause an enable ...
      --locker;                                                     // ... one level earlier than it would have. This might be surprising so an
    }
    // unmatched unlock might be the best enable.
    if(locker == 0) { // if not locked then actually enable
      strobe(0x100);
    }
  }

  /** direct disable. You probably should be using @see lock. */
  void disable(void) const {
    strobe(0x180);
  }

  /** nest lock requests, disable if first. */
  void lock(void){
    if(locker++ == 0) {
      disable();
    }
  }

  /** trigger ISR now */
  void fake(void) const {
    strobe(0x200);
  }

  /** acknowledge, to allow it to happen again */
  void clear(void) const {
    strobe(0x280);
  }

  /** clear any pending then enable, regular enable will cause an interrupt if one is pending.*/
  void prepare(void){
    clear();
    enable();
  }
};

/** disable interrupt on creation of object, enable it on destruction
 * usage: create one within a block where the irq must not be honored.
 * Note: this cheap implementation may turn on an interrupt that was off,
 * don't lock if you can't tolerate interrupts globally being enabled.
 * Since each interrupt can be stifled at its source this should not be a problem.
 * maydo: automate detection of being in the irq service and drop the inIRQ argument.
 */
struct IRQLock {
  Irq &irq;
public:
/** @param inIrq whether we are creating this object in the given @param irq's isr (or one of same priority).*/
  IRQLock(Irq &irq, bool inIrq = false): irq(irq){
    if(! inIrq) {
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
 * stm32 only implements the 4 msbs of the logic so values 3,2,1 are same as 0*/
void configurePriorityGrouping(int code); // cortexm3.s or stm32.cpp

#ifdef ALH_SIM // just compiling for syntax checking
#define EnableInterrupts
#define DisableInterrupts
#define LOCK(somename)
#define IRQLOCK(irq)
class CriticalSection { // #this stub needed as we don't want to bother #ifdef around the init of the nesting field of the useful instantiaion.
  static volatile int nesting;
};
#else
// conjecture: toggling global interrupts seems to reset something in the priority logic if done in an isr. stm32f103.
// should try to redo using priority gating rather than tickling the core logic.
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

  ~CriticalSection(void){
    if(nesting != 0) { // then interrupts are globally disabled
      if(--nesting == 0) {
        EnableInterrupts;
      }
    }
  }
};

/** disables interrupts for the duration of the block it is defined in.
 * Note: this does not allow for static locking, only for within a function's execution (which is a good thing!):*/
#define LOCK(somename) CriticalSection somename ## _locker
/** this macro can only handle simple access to the irq, no this-> or submembers: */
#define IRQLOCK(irqVarb) IRQLock IRQLCK ## irqVarb(irqVarb)
#endif /* ifdef ALH_SIM */

// the following names are too generic. should prefix with nvic_ and leave defined:
#undef strobe
#undef lvalue
#endif /* ifndef nvicH */
