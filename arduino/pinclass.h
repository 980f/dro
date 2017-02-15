#pragma once

//this ran afould of the maple ide, back tousing #defines so that thye can vary wildly between implementations
//enum IrqMode {
//  Low, High,  //used for digital i/o level
//  Change, Falling,Rising //only use for Irq mode
//};
// 
//enum PinMode { Input, Output, InputPullup }; //carefully matched to ARDUINO values


#ifndef ARDUINO
//stubs to test compile
void pinMode(unsigned pinnum, unsigned PINMODE);
bool digitalRead(unsigned pinnum);
void digitalWrite(unsigned pinnum, bool);
#endif

/* @param arduinoNumber is such as 13 for the typical LED pin.
@param mode should be one of:: INPUT(0), INPUT_PULLUP(2), and OUTPUT(1) 
@param polarity is HIGH(1) if true means output pin high, LOW(0) if true means pin low.
Via polarity you can adde an inverting buffer to a line and change that in one place. 
Also if driving a FET to operate a higher voltage load a HIGH on a pin is usually a LOW on the device.

Since this is template code the compiler should generate the traditional line of arduino code, these should have no overhead whatsoever compared to using the digitalXXX calls explicitly.

The pinMode() command which is usually in the setup() gets called where declared. 
For statically declared pins (not inside a function) this executes shortly before setup() via the c-startup code. 
This means that statically declared Pins can be used in your setup() code.
If you declare a Pin in a function then the pinMode command executes each time you enter the function.

The downside of a template based implementation is that each Pin is a class unto itself. That means you can't pass a reference to one to a routine such as a software serial port. 
There is a number() method added so that you can pass that piece of into about the pin to standard arduino code, or to your code which creates a pin locally which does a potentially pointless pinMode() but otherwise works fine.

*/

/** pretending to not know that HIGH=1 and LOW=0 ... constexpr should inline '1-active' at each place of use and not actually do a function call */
static constexpr bool inverse(bool active){
  return (HIGH+LOW)-active;
}

template <unsigned arduinoNumber,unsigned mode,unsigned polarity=HIGH> struct Pin {  

  enum Bits {
  /** in case you have to interface with something that takes the digitalXXX number*/
    number=arduinoNumber,
    active=polarity,
    inactive=inverse(polarity)
  };
  
  Pin(){
    pinMode(arduinoNumber,WiringPinMode(mode)); //MAPLE made us do this cast
  }
  
  bool get()const{
    return digitalRead(arduinoNumber)==active;
  }
  
  bool setto(bool value)const{//const is allowed as this operation doesn't change the code, only the real world pin
    digitalWrite(arduinoNumber,value?active:inactive);
    return value;
  }

  bool setto(int value)const{//const is allowed as this operation doesn't change the code, only the real world pin
    digitalWrite(arduinoNumber,(value!=0)?active:inactive);
    return value!=0;
  }
  
/// the following are utility functions, not essential to this class.
  /** using 'flip' instead of toggle due to derived classes also deriving from BoolishRef which has a 'toggle' that works for this but may not be as fast */
  bool flip() const { //const is allowed as this operation doesn't change the code, only the real world pin
    return setto(inverse(get()));
  }
  
};

/* some convenience class names:
 *  Note that the InputPin used pullup mode, it is rare that that is not what you want. 
 *  Also note that some devices have more options such as pulldown, that arduino does not provide access to.
 */
template <unsigned arduinoNumber,unsigned polarity=HIGH> struct InputPin: public Pin<arduinoNumber, INPUT_PULLUP, polarity>{
  operator bool() const {
    return Pin<arduinoNumber, INPUT_PULLUP, polarity>::get();
  }
};

#include "boolish.h" // so that it may be passed to a generic bit flipping service
template <unsigned arduinoNumber,unsigned polarity=HIGH> struct OutputPin: public Pin<arduinoNumber, OUTPUT, polarity>, public BoolishRef {
  using Pin<arduinoNumber, OUTPUT, polarity>::setto;
  
  bool operator =(bool value)const override {
    return setto(value);
  }

  //reading an output pin is ambiguous, it is not clear if you are reading the pin or the requested output value. most of the time that makes no difference so ...:
  operator bool()const {
    return Pin<arduinoNumber, OUTPUT, polarity>::get();
  }
  
  /** an active edge is one which ends up at the given polarity*/
  void edge(bool leading)const {
    if(leading){
      setto(inverse(polarity));
      setto(polarity);
    } else {
      setto(polarity);
      setto(inverse(polarity));
    }    
  }
  
};


