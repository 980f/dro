
Print &dbg(Serial);

template <Print &one,Print &two> struct PrintFork {
  template <typename Any> void print(Any thing){
    one.print(thing);
    two.print(thing);
  }
  template <typename Any> void println(Any thing){
    one.println(thing);
    two.println(thing);
  }

};

//use a macro to get variable name:
#define Show(arg) dbg.print("\n" #arg ":");dbg.print(arg)

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

#include "motorshield1.h"

const InputPin<5, HIGH> phB;
const InputPin<4, HIGH> phA;
const InputPin<3, HIGH> Index;

#include "timerservice.h"

#include "retriggerablemonostable.h"

RetriggerableMonostable lamprey(red, Ticks(350));
RegisterTimer(lamprey);
void triggerPulse(){
  lamprey.trigger();
}


#include "interruptPin.h"
void b2irqhandler() {
  green = button2;
  RX=green;
  if(button2){
    M2=1*(1-button1);//go forward unless other button is pressed in which case stop
  }
  dbg.print("G");
  dbg.print(millis());
}
const InterruptPin<b2irqhandler, button2.number, FALLING> b2irq;

void b1irqhandler(){
  red = button1;
  TX=red;
  if(button1){
    M2=-1*(1-button2);//see greenLight();
  }
  dbg.print("R");
}
const InterruptPin<b1irqhandler, button1.number, FALLING> b1irq;


/** quadrature table:
 *      ___     ___
 *  ___/   \___/    
 *        ___     ___
 *  _____/   \___/    
 *  
 *  A:HL=++
 *  A:LH=++
 *  B:HH=++
 *  B:LL=++
  */
int location=0;

void phAirqHandler(){
  if(phB==phA){
    --location;
  } else {
    ++location;
  }
}
void phBirqHandler(){
  if(phA==phB){
    ++location;
  } else {
    --location;
  }
}
const InterruptPin<phAirqHandler, phA.number, CHANGE> phAirq;
const InterruptPin<phBirqHandler, phB.number, CHANGE> phBirq;

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
class SoftPWM: public PolledTimer {
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
  
  SoftPWM(Ticks low, Ticks high, bool andStart=false){
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

 
class SlowPWMdemo: public SoftPWM {
public:
  using SoftPWM::SoftPWM;
  //it is a bad idea to do this much in an isr ...
  virtual void onToggle(bool on){
    if(!on){
      dbg.print("\nSoftPWM:");
    } else {
      dbg.print(" to ");
    }
    dbg.print(millis());
  }
    
} spwmdemo(250,750,false);

RegisterTimer(spwmdemo);

void setup() {
  SerialUSB.begin(230400);//'native' usb port
  Serial.begin(2625000);//an actual uart, on DUE DMA is used so we should be able to push the baud rate quite high without choking the processor with interrupts.
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  b2irq.attach(true);//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  b1irq.attach(true);
  M2=0;
  M1=0;
  phAirq.attach(false);
  phBirq.attach(false);
}

int lastlocation=0;
void loop() {
  __WFE();
  if(changed(lastlocation,location)){
    //initially this printed sequential values, but eventually only every 6 or 7, then choked.Will try usbserial again, maybe it can keep up.
    dbg.print('\n');
    dbg.print(location);
  }
}
