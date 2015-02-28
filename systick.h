#ifndef systickH
#define systickH
#include "eztypes.h"
#include "cheapTricks.h"
/** start ticking at the given rate.
 * presumes system clock is already programmed and that it is the clock source*/
void startPeriodicTimer(u32 persecond);

/** time since last rollover (<1 ms if 1kHz, even shorter at higher sysfreq), only suitable for spinning in place for a short time.
 *  NB: this is NOT in ticks, it is in probably 8MHz increments, but that may differ for some clock configurations.*/
u32 snapTime(void);
/** much longer time range but an expensive call, in units of system tick, call secondsForTicks()*/
u32 snapTickTime(void);
/** much much longer time range, with a range greater than the life of the instrument., call secondsForLongTime()*/
u64 snapLongTime(void);

/**
 * an isr will determine that the given time has expired,
 * but the interested code will have to look at object to determine that the event occurred.
 * IE: code after the WFE/WFI must look at the done bit.
 */
class PolledTimer {
public:
  bool done;
  u32 systicksRemaining;
  PolledTimer(void);
  /** set timeout value and start the timer */
  virtual void restart(u32 ticks); // add to list if not present, never gets removed so don't add dynamic objects.

  /** set timeout value and start the timer */
  void restart(float seconds); // float not double as is often in time critical code.
  /** make it look as if time completed. Does NOT call the ondone virtual method */
  void freeze(){
    done = 1; // precludes isr touching remaining time, and onDone doesn't get called.
  }
public: // accessors for the system timer (presently systick, but that is encapsulated)
  static double secondsForTicks(u32 ticks);
  static double secondsForLongTime(u64 ticks);

  /** @returns ticks necessary to get a delay of @param sec seconds*/
  static u32 ticksForSeconds(float sec);
  static u32 ticksForMillis(int ms);
  static u32 ticksForMicros(int us);
  /** @returns ticks necessary to get a periodic interrupt at the @param hz rate*/
  static u32 ticksForHertz(float hz); // approximate since we know a divide is required.
private:
  // singly linked list
  PolledTimer *next;
  static PolledTimer *active; // isr needs to know where the list is.
public:
  static void onTick(void); // to be called periodically, expects to be called in an isr.
  // extend and overload to have something done within the timer interrupt service routine:
  virtual void onDone(void);
};

/** automatic restart. If you are slow at polling it it may become done again while you are handling a previous done.*/
class CyclicTimer: public PolledTimer {
protected:
  u32 period;
  u32 fired;
public:
  bool hasFired(void){
    // maydo: LOCK, present intended uses can miss a tick without harm so we don't bother.
    ClearOnExit<u32> z(fired); // trivial usage of ClearOnExit, for testing that.
    return fired != 0;
  }

  operator bool(void){
    return hasFired();
  }

  void retrigger(void){
    // leave fired as is.
    PolledTimer::restart(period);
  }

  void restart(u32 ticks){
    PolledTimer::restart(period = ticks);
  }

  virtual void onDone(void){
    ++fired;
    // pointless: PolledTimer::onDone();
    retrigger();
  }
};

#endif /* ifndef systickH */
