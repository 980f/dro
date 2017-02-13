#ifndef QUADRATER_H
#define QUADRATER_H

#include "Arduino.h"

/** quadrature table:
 *      ___     ___
 *  ___/   \___/
 *        ___     ___
 *  _____/   \___/
 *
 *  A:HL=++
 *  A:LH=++
 *  B:HH=++
 *  B:LL=++
 */

/** base class for sharing across templated counter assemblies */
class QuadratureCounter {
public://eventually protect
  int location = 0;
  int dirstep = 0;
public:
  template<bool primary> void step(bool samePhase){
    if(primary) {
      if(samePhase) {
        --location;
      } else {
        ++location;
      }
    } else {
      if(samePhase) {
        ++location;
      } else {
        --location;
      }
    }
  } /* step */

  /** when cruising we only look at one phase, and possible one edge.
   * the dirstep records the direction and if one edge then set to 4, if both edges set to 2.
   */
  void cruiseDirection(int dirstep){
    this->dirstep = dirstep;
  }

  /** call this with each primary edge when mid-rate cruising */
  void cruiseStep(){
    location += dirstep;
  }

  /** when using a hardware counter we retain the cruiseDirection setting and report the merge of the last incremental update with the count value.*/
  int fly(unsigned count) const {
    return location + dirstep * count;
  }

  /** when a 16 bit hardware counter wraps we need to update our internal value (unless we keep a rollover count with the counter).
   * Also if we reset the hardware counter such as with an index mark, we need to accumulate the pre-cleared value.
   * Unless the index is an absolute clear. */
  void rollover(unsigned count){
    location += dirstep * count;
  }

} ;

#include "hook.h"
#include "pinclass.h"
#include "interruptPin.h"

template<unsigned pinA, unsigned pinB,unsigned pinIndex> class Quadrater {
//public:
  static QuadratureCounter pos;
protected:
//need forward references to change functions, so use a Hook to indirect those.
  static HookOnce<bool /*otherPin*/> cruiseChange;

  static const InputPin<pinB, HIGH> phB;   //5
  static const InputPin<pinA, HIGH> phA;   //4
//    const InputPin<3, HIGH> Index; //3


//change to cruise mode must be synched with isr, so set a bit and let slow isr do the switch:
  static void phAirqHandler(){
    pos.step<true>(phA==phB);
    if(phA) {
      cruiseChange(phB);
    }
  }

  static void phFastHandler(){
    pos.cruiseStep();
    cruiseChange(phB);
  }

  static void phBirqHandler(){
    pos.step<false>(phA==phB);
    //we never change sensing modality on B events, makes life much simpler and would provide no real benefit.
  }
  static const InterruptPin<phAirqHandler, phA.number, CHANGE> phAirq;
  static const InterruptPin<phBirqHandler, phB.number, CHANGE> phBirq;
  //using seperate entity for different interrupt style until we see how an attachInterrupt goes about its business. 
  static const InterruptPin<phFastHandler, phA.number,FALLING> phFast;

/** isr part of going to cruise mode */
  static void startCruise(bool phb){
    phBirq = 0;
    pos.cruiseDirection(phb ? +4 : -4);
    phFast = 1;
  }

/** isr part of leaving cruise mode */
  static void endCruise(bool /*phb*/){
    phFast = 1;
    phAirq = 1;
    phBirq = 1;
  }

public:
  static void beginCruising(){
    cruiseChange = startCruise;
  }

  static void endCruising(){
    cruiseChange = endCruise;
  }

  static void connect(){
    endCruise(0);
  }

  int position() const {
    return pos.location;
  }

}; // class Quadrater

//and the following is one of the reasons some people despise C++, static members must be 'redundantly' described in order to actually exist.
template<unsigned pinA, unsigned pinB,unsigned pinIndex>
QuadratureCounter Quadrater< pinA,  pinB, pinIndex> ::pos;

template<unsigned pinA, unsigned pinB,unsigned pinIndex>
HookOnce<bool /*otherPin*/> Quadrater<pinA, pinB, pinIndex>::cruiseChange;

#endif // QUADRATER_H
