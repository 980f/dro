#ifndef SHADOW_H
#define SHADOW_H

#include "eztypes.h"
/**
hardware registers that don't tolerate byte writes can't be directly accessed via C bit fields.
The below only fully works on registers none of whose bits change spontaneously (no status bits, or read-only).
Use bitband bits as much as possible instead of this.
*/

template <typename Fielder,typename U>
class Shadowed {
  volatile U &raw;
public:
  Fielder field;
public:
  Shadowed(u32 *address):raw(*reinterpret_cast<volatile U *>(address)){
    pun(U,field)=raw;
  }
  void update(void){
    raw=pun(U,field);
  }
};

#endif // SHADOW_H
