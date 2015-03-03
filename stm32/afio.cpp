#include "afio.h"
#include "bitbanger.h"

AfioManager theAfioManager InitStep(InitHardware-1); //just before ports and pins
AfioManager ::AfioManager():
  APBdevice(2, 0),
  b(*reinterpret_cast <AfioBand *> (bandAddress)),
  remap(registerAddress(4)){
  init();
}

void AfioManager::selectEvent(const Pin &pin){
  //4 per word, 0th at 8
  u32 &gangof4(*registerAddress(8+(pin.bitnum&~3)));//4 fields per register, 4 bytes per register= ignore 2 lsbs of port's bit number
  u32 value(pin.port.slot-2);//port A is slot 2.
  mergeBits(gangof4,value,pin.bitnum&3,4);//2 lsbs of port's bitnumber select 4 bit field
}
