#include "afio.h"

AfioManager theAfioManager InitStep(InitHardware); //somehow things worked for ages without this.
AfioManager ::AfioManager(): APBdevice(2, 0), Shadowed<AfioRemap,u32>(reinterpret_cast<unsigned>(registerAddress(4))), b(*reinterpret_cast <AfioBand *> (getBand())){
  init();
}
