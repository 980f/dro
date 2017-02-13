#ifndef MOTORSHIELD1
#define MOTORSHIELD1

/** seeed motor shield v1.0 */
template <unsigned e,unsigned f,unsigned b> struct Hbridge {
const OutputPin<e> Enab;
const OutputPin<f> Forward;
const OutputPin<b> Reverse;
public:
  /* taking an int for future use in setting a pwm on the enable pin */
  void operator =(int dir){
    Enab=false;
    Forward=dir>0;
    Reverse=dir<0;
    Enab=dir!=0;
  }
  
  void freeze(){
     Reverse=true;
     Forward=true;
     Enab=true;
  }
  
};

Hbridge<10,12,13> M2;
Hbridge<9,8,11> M1;
//end seeed motor shield v1.0
#endif
