#include "p103_board.h"

//only clocks module should care
const u32 EXTERNAL_HERTZ=8000000;

P103_board::P103_board():
  ledPin(PC,12),
  led(ledPin),
  buttonPin(PA,0),
  button(buttonPin){
  /*empty*/
}
