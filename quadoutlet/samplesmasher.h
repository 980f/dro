#ifndef MOTORSHIELD1
#define MOTORSHIELD1

#include "pinclass.h"

/** sample smasher pulled from my parts bin.
M1 uses eable and direction, M0, which I will call M2 so I can switch back and forth with motorshield, uses panic and direction.
 */
template <unsigned e,unsigned f> struct Hbridge2 {
const OutputPin<e,LOW> Enab;
const OutputPin<f> Forward;

public:
  /* taking an int for future use in setting a pwm on the enable pin */
  int operator =(int dir){
    Enab=false;
    Forward=dir>0;
    Enab=dir!=0;
    return dir;
  }
  
  operator int () const {
    if(Enab){
      return Forward?1:-1;
    } else {
      return 0;
    }
  }
  
  void freeze(){
     Forward=true;//doesn't matter but I like to drive it high for power a miniscule power reduction
     Enab=false;
  }
    
};

Hbridge2<10,11> M1;
Hbridge2<9,8> M2;

#endif
