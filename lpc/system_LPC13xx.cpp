/**
* replaces arm system_LPC13xx.c
* most of that functionality is done in this system via constructing certain objects using initPriority settings.
*/

#include "gpio.h"
using namespace LPC;

void SystemInit (void){
  GPIO::Init(); // needed before static constructors for its defined classes run.
  //leaving clock at power up default.
}
