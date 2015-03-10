#include "p103_board.h"

#include "clocks.h"

//only clocks module should care
const u32 EXTERNAL_HERTZ=8000000;//geez lousie, without 'seeing' the extern directive in clocks.h the compiler forced this to be private.

P103_board::P103_board():
  ledPin(PC,12),
  led(ledPin),
  buttonPin(PA,0),
  button(buttonPin){
  /*empty*/
}

void P103_board::toggleLed(){
  led.toggle();//low level access has optimal way of doing this.
}
