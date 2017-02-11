//#include "sam3xcounter.h"
Print &dbg(SerialUSB);

#include "pinclass.h"

/** attached to the high side of an LED */
//const OutputPin<LED_BUILTIN> lamp;//13, also a motor control line on motor shield
/** by putting a LOW in the items below, a true turns the lamp on */
const OutputPin<PIN_LED_RXL,LOW> RX;  //72,73
const OutputPin<PIN_LED_TXL,LOW> TX;

//olimex protoshield:
const OutputPin<3> red;
const OutputPin<4> green;
const InputPin<6, LOW> button1;
const InputPin<7, LOW> button2;

//seeed motor shield v1.0
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

#include "polledtimer.h"
extern "C" int sysTickHook(){
  PolledTimerServer();//all of our millisecond stuff hangs off of here.
  return false; //allowed standard code to run
}
#include "tableofpointers.h"
#include "retriggerablemonostable.h"

RetriggerableMonostable lamprey(red, Ticks(350));
RegisterTimer(lamprey);

void triggerPulse(){
  lamprey.trigger();
}

#define Show(arg) SerialUSB.print("\n" #arg ":");SerialUSB.print(arg)

#include "interruptPin.h"
void greenLight() {
  green = button2;
  RX=green;
  M2=1*(1-button1);//go forward unless other button is pressed in which case stop
  Serial.print("G");
}
const InterruptPin<greenLight, button2.number, CHANGE> greenirq;

void redLight(){
  red = button1;
  TX=red;
  M2=-1*(1-button2);//see greenLight();
  Serial.print("R");
}
const InterruptPin<redLight, button1.number, CHANGE> redirq;
//const InterruptPin<triggerPulse,button1.number,FALLING> redirq;
  

//example of acting on timing event within the timer isr:
class Flasher: public CyclicTimer {
  using CyclicTimer::CyclicTimer;
  void onDone(void) override {
    CyclicTimer::onDone();
    RX.toggle();
  }
} flashLamp(451,false);
RegisterTimer(flashLamp);


/**
 * this could have been implemented with two PolledTimer's triggering each other, but that uses more critical resources than coding it directly.
*/
class SlowPWM: public PolledTimer {
protected:
  Ticks pair[2];
  bool phase;
public:
  operator bool()const {
    return phase;
  }

  void setPhase(bool highness,Ticks ticks){
    pair[highness]=ticks?:1;
  }
  
  void setCycle(Ticks low, Ticks high){
    pair[0]=low;
    pair[1]=high;   
  }
  
  SlowPWM(Ticks low, Ticks high, bool andStart=false){
    setCycle(low,high);
    if(andStart){
      onDone();
    }
  }

  void onDone(void) override {
    phase ^=1;
    restart(pair[phase]-1);//# the polledtimer stuff adds a 1 for good luck, we don't need no stinking luck. //todo: guard against a zero input 
    onToggle(phase);
  }

  //since onDone is virtual we make this virtual also. We should try to replace that with a function member.
  virtual void onToggle(bool on){
    // a hook, overload with a pin toggle to make a PWM output.
  }
  
};

 
class SlowPWMdemo: public SlowPWM {
public:
  using SlowPWM::SlowPWM;
  //it is a bad idea to do this much in an isr ...
  virtual void onToggle(bool on){
    if(!on){
      dbg.print("\nSlowpwm:");
    } else {
      dbg.print(" to ");
    }
    dbg.print(millis());
  }
    
} spwmdemo(250,750,true);

RegisterTimer(spwmdemo);

void setup() {
  SerialUSB.begin(230400);//'native' usb port
  Serial.begin(115200);//an actual uart
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  greenirq.attach(true);//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  redirq.attach(true);
  M2=0;
  M1=0;
}

char indicator='-';
void loop() {
  __WFE();
}
