#ifndef GPIO_H
#define GPIO_H

#include "stm32.h"

/** the 16 bits as a group.
  * Note well that even when the group is not enabled the port can be read from (as long as it exists).
  *
  * For pins that are optional to a module use (const Pin *) parameters a pass nulls. Trying to create safely non-functional pins is expensive and the check for 'has a pin' is the same cost, but only burdens those pin uses which ca be optionally present. There are usually some port bits that aren't pinned out which can be used as dummies when a null pointer to a pin just isn't convenient.
  */

struct Port /*Manager*/ : public APBdevice {
  static bool isOutput(unsigned pincode){
    return (pincode&3)!=0;//if so then code is Alt/Open
  }

  /** a contiguous set of bits in a a single Port */
  struct Field {
    volatile u16& odr;
    volatile u32& at;
    const unsigned lsb;
    const unsigned mask; //derived from width
    Field(unsigned pincode,const Port &port, unsigned lsb, unsigned msb);
    /** insert @param value into field, shifting and masking herein, i.e always lsb align the value you supply here */
    void operator =(u16 value)const;
    /** toggle the bits in the field that correspond to ones in the @param value */
    void operator ^=(u16 value)const;
    /** @returns the value set by the last operator =, ie the requested output- not what the physical pins are reporting. */
    operator u16() const;
    /** read back the actual pins */
    u16 actual() const;
    //more operators only at need
  };

  /** @param letter is the uppercase character from the stm32 manual */
  Port(const char letter);
  /**
    * configure the given pin.
    todo:M enumerize the pin codes (but @see InputPin and OutputPin classes which construct codes for you.)
    */
  void configure(unsigned bitnum, unsigned code) const;
  volatile u16 &odr(void) const;
};


//these take up little space and it gets annoying to have copies in many places.
extern const Port PA;
extern const Port PB;
extern const Port PC;
extern const Port PD;
extern const Port PE;
//extern const Port F;
//extern const Port G;

/**
  * this class manages the nature of a pin, and provides cacheable accessors for the pin value.
  * there is no point in using const in declaring a Pin, the internals are all const.
  */
struct Pin /*Manager*/ {
  const u8 bitnum; //0..15, using small type so that object can be passed in an 32 bit register.
  const Port port; //copy, for access efficiency (it is smaller than a pointer!), Port objects are inherently const so there is no ambiguity in using a copy.

  void output(unsigned int code, unsigned int mhz, bool openDrain) const; /* output */

  Pin(const Port &port, const unsigned int bitnum): bitnum(bitnum), port(port){/*empty*/}
  /** @returns this after configuring it for analog input */
  const Pin& AI(void) const;
  /** @returns bitband address for input after configuring as digital input, pull <u>U</u>p, pull <u>D</u>own, or leave <u>F</u>loating*/
  volatile u32 &DI(char UDF = 'D') const;
  /** @returns bitband address for controlling high drive capability [rtfm] */
  volatile u32 &highDriver(void) const;
  /** configure as simple digital output */
  volatile u32 &DO(unsigned int mhz = 2, bool openDrain = false) const;
  /** configure pin as alt function output*/
  const Pin& FN(unsigned int mhz = 2, bool openDrain = false) const;
  /** declare your variable volatile, reads the actual pin, writing does nothing */
  volatile u32 &reader(void) const;
  /** @returns reference for writing to the phyiscal pin, reading this reads back the DESIRED output */
  volatile u32 &writer(void) const;

  /** for special cases, try to use one of the above which all call this one with well checked argument */
  void configureAs(unsigned int code) const;
};

/** base class for InputPin and OutputPin that adds polaarity at construction time */
class LogicalPin {
protected:
  volatile u32& bitbanger;
  /** a 1 for LOWACTIVE output, ie: setting this element to a 1 sets the physical pin low */
  const u32 lowactive; //not templated as we want to be able to pass Pin's around. not a hierarchy as we don't want the runtime cost of virtual table lookup.
  inline u32 polarized(bool operand)const{
    return lowactive^operand;
  }

  LogicalPin(volatile u32 &registerAddress,bool inverted);
public:

  /** @returns for outputs REQUESTED state of pin, for inputs the acutal pin */
  inline operator bool(void)const{
    return polarized(bitbanger);
  }
};

/**
hide the volatile and * etc that I sometimes forget.
maydo: retain UDF setting as value to return in operator bool ternary.
*/
class InputPin :public LogicalPin {

public:
  InputPin(const Pin &pin, char UDF = 'D', bool lowactive=false);
  InputPin(const Pin &pin,bool lowactive);  //pull the opposite way of the 'active' level.
//maydo: add method to change pullup/pulldown bias while running

};

/**
a digital output made to look like a simple boolean.
Note that these objects can be const while still manipulating the pin.
*/
class OutputPin :public LogicalPin {

public:
  OutputPin(const Pin &pin,bool lowactive=false,unsigned int mhz = 2, bool openDrain = false);

  /** @returns pass through @param truth after setting pin to that value */
  inline bool operator = (bool truth)const{
    bitbanger=polarized(truth);
    return truth;//don't reread the pin, nor its actual, keep this as a pass through
  }

  /** set to given value, @returns whether a change actually occured.*/
  inline bool changed(bool truth) const{
    truth=polarized(truth);
    if(bitbanger!=truth){
      bitbanger=truth;
      return true;
    }
    return false;
  }

  inline bool actual() const {
    return polarized((&bitbanger)[-32]);//idr is register preceding odr, hence 32 bits lower in address
  }
};

#endif // GPIO_H
