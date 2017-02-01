#pragma once

#ifndef ARDUINO
#define HIGH 1
#define LOW 0
#define INPUT_PULLUP 1
#define OUTPUT 0
void pinMode(unsigned, unsigned);
bool digitalRead(unsigned);
void digitalWrite(unsigned, bool);
#endif
/* @param arduinoNumber is such as 13 for the typical LED pin.
@param mode should be one of:: INPUT(0), INPUT_PULLUP(2), and OUTPUT(1) 
@param polarity is HIGH(1) if true means output pin high, LOW(0) if true means pin low.
Via polarity you can adde an inverting buffer to a line and change that in one place. 
Also if driving a FET to operate a higher voltage load a HIGH on a pin is usually a LOW on the device.

Since this is template code the compiler should generate the traditional line of arduino code, these should have no overhead whatsoever compared to useing the digitalXXX calls explicitly.

The pinMode() command which is usually in the setup() gets called where declared. 
For statically declared pins (not inside a function) this executes shortly before setup() via the c-startup code. 
This means that statically declared Pins canbe used in your setup() code.
If you declare a Pin in a function then the pinMode command executes each time you enter the function.

The downside of a template based implementation is that each Pin is a class unto itself. That means you can't reasonable pass a reference to one to a routine such as a software serial port. 
There is a number() method added so that you can pass that piece of into about the pin to standard arduino code.

*/
/** pretending to not know that HIGH=1 and LOW=0 ... constexpr should inline '1-active' at each place of use and not actually do a function call */
static constexpr bool inverse(bool active){
  return HIGH+LOW-active;
}

template <unsigned arduinoNumber,unsigned mode,unsigned polarity=HIGH> struct Pin {  

  enum Bits {
  /** in case you have to interface with something that takes the digitalXXX number*/
    number=arduinoNumber,
    active=polarity,
    inactive=inverse(polarity)
  };
  
  Pin(){
    pinMode(arduinoNumber,mode);
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
  
/// the following are utility functions, not essential to this class
  bool toggle()const { //const is allowed as this operation doesn't change the code, only the real world pin
    return setto(inverse(get()));
  }
  
};

/* some convenience class names:
 *  Note that the InputPin used pullup mode, it is rare that that is not what you want. 
 *  Also note that some devices have more options such as pulldown, that arduino does not provide access to.
 */
template <unsigned arduinoNumber,unsigned polarity=HIGH> struct InputPin: public Pin<arduinoNumber, INPUT_PULLUP, polarity>{
  operator bool()const {
    return Pin<arduinoNumber, INPUT_PULLUP, polarity>::get();
  }
};

template <unsigned arduinoNumber,unsigned polarity=HIGH> struct OutputPin: public Pin<arduinoNumber, OUTPUT, polarity>{
  using Pin<arduinoNumber, OUTPUT, polarity>::setto;
  
  bool operator =(bool value)const {
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


