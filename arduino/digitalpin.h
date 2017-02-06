
/** while this variation of Pin (see pinclss.h) can be passed by reference, which includes being able to put into an array.
 *  It generates more code at each point of use. An aggressively optimizing compiler might eliminate that extra code, 
 *  but for the template version of this functionality the compiler most likley will write minimal code without being asked nicely */
class DigitalPin {
public:
  const unsigned number;
  const unsigned polarity;

DigitalPin(unsigned arduinoNumber,unsigned mode,unsigned polarity=HIGH):
  number(arduinoNumber),
  polarity(polarity){
    pinMode(arduinoNumber,mode);
  }
  
  operator bool()const{
    return digitalRead(number)==polarity;
  }
  
  bool operator =(bool value)const{
    digitalWrite(number , value ? polarity : HIGH+LOW-polarity);
    return value;
  }
/// the following are utility functions, not essential to this class
  bool toggle()const {
    *this = ! *this;
     return operator bool();//FYI you can call operator overloads as if they were normal functions.
  }
  
};

/* some convenience classes:
 *  Note that the InputPin used pullup mode, it is rare that that is not what you want. 
 *  Also note that some devices have more options such as pulldown, that arduino does not provide access to.
 */
class DigitalInput: public DigitalPin {
  public:
  DigitalInput(unsigned arduinoNumber,unsigned polarity=HIGH):
  DigitalPin(arduinoNumber, INPUT_PULLUP, polarity){}
};

class DigitalOutput: public DigitalPin {
  public:
  DigitalOutput(unsigned arduinoNumber,unsigned polarity=HIGH):
  DigitalPin(arduinoNumber, OUTPUT, polarity){}
};
