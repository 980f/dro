
//declares an asm routine that can be accessed via C without much pain. C prototype will need 'extern "C"' treatment.
.macro Cfunction fname
  .global \fname
  .thumb
  .thumb_func

\fname :
  .endm
//todo: put the macros in a file to be included.

  .align 4   //align nanoSpin so that flash cache works predictably
  Cfunction nanoSpin
  //todo: rework so that the 83 comes from the clock setting code.
  //the '83' is 6 clocks at 72Mhz. 
  sub r0,#83
  bpl nanoSpin
  bx lr

//r0 will be address, trust that it is 32-bit aligned
  Cfunction atomicDecrementAttempt
  ldrex r1,[r0]
  add r1, r1, #1
  strex r0,r1,[r0]
  bx lr  


.end
