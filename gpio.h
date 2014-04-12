/*****************************************************************************
 *   gpio.h:  Header file for NXP LPC13xx Family Microprocessors
 *
 *   c++ version of LPC stuff, thoroughly typed
 * and then a purely c++ performance tweaked version (when objects can be statically constructed).
******************************************************************************/
#ifndef GPIO_H 
#define GPIO_H

/** the ports are numbered from 0. Making them unsigned gives us a quick bounds check via a single compare*/
typedef unsigned PortNumber;
inline bool isLegalPort(PortNumber pn){
  return pn<4;
}

typedef unsigned BitNumber;

class GPIO {
private:
  const PortNumber portNumber;
  const BitNumber bitNumber;
public:
  GPIO(PortNumber portNum, BitNumber bitPosition):
    portNumber(portNum),
    bitNumber(bitPosition)
  {
  }
  bool operator =(bool setHigh) const{
    SetValue(portNumber,bitNumber,setHigh);
    return setHigh;
  }
  operator bool() const {
    return GetValue(portNumber,bitNumber);
  }
public:
  /** this must be called once before any other functions herein are used. SystemInit is a good place, so that constructors for outputs will work.*/
  static void Init( void );
public: //cmsis/ lpcopen legacy;
  static void SetDir( PortNumber portNum, BitNumber bitPosi, bool outputter);
  static void SetValue( PortNumber portNum, BitNumber bitPosi, bool bitVal );
  /** @returns actual pin value, not necessarily last value set. */
  static bool GetValue( PortNumber portNum, BitNumber bitPosi);

  static void SetInterrupt( PortNumber portNum, BitNumber bitPosi, uint32_t sense, uint32_t single, uint32_t event );
  static void IntEnable( PortNumber portNum, BitNumber bitPosi );
  static void IntDisable( PortNumber portNum, BitNumber bitPosi );
  static bool IntStatus(PortNumber portNum, BitNumber bitPosi );
  static void IntClear( PortNumber portNum, BitNumber bitPosi );

};
#endif /* end __GPIO_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
