#ifndef UART_H
#define UART_H
//#include "eztypes.h"

namespace LPC {

/** uart manager class, not to be confused with hardware register class.
Extend/derive from it for interrupt driven usage.
*/

class Uart {
  /** the isr. a stub is locally generated to vector to it */
  void isr(void);
public:
  Uart();
  /** @param hertz is used to compute multiple register settings to get the closest to the desired value.
@returns the actual rate. */
  unsigned setBaud(unsigned hertz)const;
  /** @param coded e.g. "8N1" for typical binary protocol, 8E2 for modbus */
  void setFraming(const char*coded)const;
protected: //
  /** called by isr on an input event.
   * negative values of @param are notifications of line errors, -1 for interrupts disabled */
  virtual bool receive(int incoming);
  /** called by isr when transmission becomes possible.
@return either an 8 bit unsigned character, or -1 to disable transmission events*/
  virtual int send();
  /** called to request that transmit isr be enabled or disabled.
   * Note: if enabled and wasn't enabled then send() will immediately be called to get first byte to send, IE send() isn't only called from isr (unless we can arrange for that!). */
  void transmission(bool enabled=true);
  /** call to request that reception isr be enabled or disabled. */
  void reception(bool enabled=true);
};

}

#endif // UART_H
