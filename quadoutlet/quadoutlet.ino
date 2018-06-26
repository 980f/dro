//demo of relay board

//coming soon #include "leonardo.h"
Print &dbg(SerialUSB);//on leonardo this is the USB, Serial1 is hardware port
Stream &console(SerialUSB);

//end leonardo.h
//use a macro to get variable name:
#define Show(arg) dbg.print("\n" #arg ":"); dbg.print(arg)

#include "pinclass.h"
OutputPin<4> wtf;


#include "relayshield.h"

void alloff(){
  R1=0; 
  R2=0;  
  R3=0; 
  R4=0;
}
/** attached to the high side of an LED */
const OutputPin<LED_BUILTIN> lamp;//13, also a motor control line on motor shield


#include "timerservice.h"
//
//#include "retriggerablemonostable.h"
//
//RetriggerableMonostable lamprey(red, Ticks(350));
//RegisterTimer(lamprey);
//void triggerPulse(){
//  lamprey.trigger();
//}

//example of acting on timing event within the timer isr:
class Flasher: public CyclicTimer {
  using CyclicTimer::CyclicTimer;
  void onDone(void) override {
    CyclicTimer::onDone();
    lamp.toggle();
  }

} flashLamp(1451,false);

RegisterTimer(flashLamp);

#include "softpwm.h"
class SlowPWMdemo: public SoftPWM {
public:
  using SoftPWM::SoftPWM;
  //it is a bad idea to do this much in an isr ... but it's not an isr on a leonardo.
  virtual void onToggle(bool on){
    R1=on;
//    if(!on) {
//      dbg.print("\nSoftPWM:");
//    } else {
//      dbg.print(" to ");
//    }
//    dbg.print(millis());
  }

} spwmdemo(2000,3000,false);

RegisterTimer(spwmdemo);

void setup(){
  SerialUSB.begin(115200);//'native' usb port of leonardo
//  Serial2.begin(115200);//an actual uart, on DUE DMA is used so we should be able to push the baud rate quite high without choking the processor with interrupts.
  R1.reinit();
  R2.reinit();
  R3.reinit();
  R4.reinit();
  
//  flashLamp.retrigger();
  spwmdemo.restart(Ticks(10));
} /* setup */


//extern unsigned millis();//todo:1 why is this not in scope?--moved code here to get it found
void pollTicker(){
  static unsigned previous=millis();
  unsigned now=millis();
  if(now<previous){//wrapped
    now+=1+~previous;//if previous = 1111 and now 0000 want prev=000 and now 0001   
    previous=0;
  } 
  while(previous++<now){
    PolledTimerServer();
  }
}

void loop(){
  pollTicker();//check millis
  if(console.available()){
    char c=console.read();
    console.print(c);
    switch(c){
      case 27: alloff(); break;
      case '1': R1.toggle(); break;
      case '2': R2.toggle(); break;
      case '3': R3.toggle(); break;
      case '4': R4.toggle(); break;
      case 'q': R1=0; break;
      case 'w': R2=0; break;
      case 'e': R3=0; break;
      case 'r': R4=0; break;
      case 'a': R1=1; break;
      case 's': R2=1; break;
      case 'd': R3=1; break;
      case 'f': R4=1; break;
      case ' ': 
        dbg.print(R1);
        dbg.print(R2);
        dbg.print(R3);
        dbg.print(R4);
        dbg.print(':');
        break; //print out state
    }
  }
  
}
