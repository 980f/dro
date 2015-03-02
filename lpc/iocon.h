#ifndef IOCON_H
#define IOCON_H

#include "lpc13xx.h"

namespace LPC {
// todo: implement the 4 special bits where they belong and get rid of the last dregs of iocon.
/** this function hides the essentially random address for the configuration code for a port pin */
  void setPortMode(unsigned PortNumber, int pinNumber, int modecode);
}
#endif // IOCON_H
