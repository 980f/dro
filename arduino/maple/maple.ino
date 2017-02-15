//thjis is maple version of dro project.

Print &dbg(Serial2);//on leonardo this is the USB, Serial1 is hardware port
Stream &console(Serial2);

//use a macro to get variable name:
#define Show(arg) dbg.print("\n" #arg ":"); dbg.print(arg)

#include "pinclass.h"

/** attached to the high side of an LED */
//const OutputPin<LED_BUILTIN> lamp;//13, also a motor control line on motor shield

////due:
//const OutputPin<LED_BUILTIN_RX> RX;// 17
//const OutputPin<LED_BUILTIN_TX>TX; // 30
CachedBoolish RX;
CachedBoolish TX;

//#include "motorshield1.h"
#include "olimexprotov1.h" 

#include "samplesmasher.h"

#include "quadrater.h" // quadrature tracker.
Quadrater<4,5,3> tracker;

#include "timerservice.h"
//
//#include "retriggerablemonostable.h"
//
//RetriggerableMonostable lamprey(red, Ticks(350));
//RegisterTimer(lamprey);
//void triggerPulse(){
//  lamprey.trigger();
//}

#include "interruptPin.h"
void b2irqhandler(){
  RX = button2;
  if(button2) {
    //analogWrite(10,100);
    M1 = M2 = 1 * (1 - button1);//go forward unless other button is pressed in which case stop
  }
  dbg.print("G");
  dbg.print(millis());
}

const InterruptPin<b2irqhandler, button2.number, FALLING> b2irq;

void b1irqhandler(){
  TX = button1;
  if(button1) {
    //analogWrite(10,200);
    M1 = M2 = -1 * (1 - button2);//see greenLight();
  }
  dbg.print("R");
}

const InterruptPin<b1irqhandler, button1.number, FALLING> b1irq;

//example of acting on timing event within the timer isr:
class Flasher: public CyclicTimer {
  using CyclicTimer::CyclicTimer;
  void onDone(void) override {
    CyclicTimer::onDone();
//    RX.toggle();
  }

} flashLamp(451,false);
RegisterTimer(flashLamp);

#include "softpwm.h"
class SlowPWMdemo: public SoftPWM {
public:
  using SoftPWM::SoftPWM;
  //it is a bad idea to do this much in an isr ...
  virtual void onToggle(bool on){
    if(!on) {
      dbg.print("\nSoftPWM:");
    } else {
      dbg.print(" to ");
    }
    dbg.print(millis());
  }

} spwmdemo(250,750,false);

RegisterTimer(spwmdemo);

void setup(){
//  SerialUSB.begin(230400);//'native' usb port
  Serial2.begin(115200);//an actual uart, on DUE DMA is used so we should be able to push the baud rate quite high without choking the processor with interrupts.
  //Pin structs take care of themselves, unless you need special modes outside arduino's libraries.
  b2irq.attach(true);//we don't build in attach() to the constructor as in many cases the isr needs stuff that isn't initialized until setup() is run.
  b1irq.attach(true);
  M1 = M2 = 0;
  tracker.connect();
} /* setup */

int lastlocation = 0;
void loop(){
//  __WFE();
  if(!(millis()%1000)){
    dbg.print("\nstill alive ");
  }
  if(changed(lastlocation,tracker.position())) {

    //initially this printed sequential values, but eventually only every 6 or 7, then choked.Will try usbserial again, maybe it can keep up.
    dbg.print('\n');
    dbg.print(lastlocation);
  }
  if(lastlocation>500){
    tracker.beginCruising();
  }
  if(console.available()){
    char c=console.read();
    console.print(c);
    switch(c){
      case '<': M1 = M2 = 1; break;
      case '>': M1 = M2 = -1; break;
      case 27: M1 = M2 = 0; break;
      case '?': console.println(tracker.position()); break;
    }
  }
  
}
