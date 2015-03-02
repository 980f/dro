#include "p103_board.h"

//only clocks module should care
const u32 EXTERNAL_HERTZ=8000000;

P103_board::P103_board():
  led(Pin(PC,12)),
  button(Pin(PA,0))
{
  /*empty*/
}
