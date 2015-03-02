#include "timer.h"

/** two monostables with shared trigger.
  * implementing via two compares on a common counter*/
class PairedOneshots : public Timer {
public:
  CCUnit shorter;
  CCUnit longer;

  /**@param luno which hardware timer to use,longLuno&shortLuno 1.4 for cc pins*/
  PairedOneshots(int luno, int longLuno, int shortLuno);
  /**set up the paired oneshots for the given clocking resolution.
    * to set the ticks required for each oneshot you will have to remember what you set here and multiply the time delay desired in seconds by the value entered here.
    * the smallest functioning value is ~1100 (given 72MHz processor).*/
  void init(int baseFrequency);

  /** restart timeout*/
  inline void retrigger(void){
    startRunning(); //as long as this clears all possible interrupt sources then our retrigger can use it.
  }

  /** expects to be called from an isr. Checks and clears source flags
    * @return 2 for long fired, 1 for short fired, 0 for neither is fired (sw error!)*/
  int whichFired(void);

/** set up one of the oneshots.*/
  void initCC(CCUnit&cc, int ticks);
};
