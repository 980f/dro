
//declares an asm routine that can be accessed via C without much pain. C prototype will need 'extern "C"' treatment.
.macro Cfunction fname
  .global \fname
  .thumb
  .thumb_func

\fname :
  .endm
//todo: put the macros in a file to be included.

  .align 4   //align nanoSpin so that flash cache works predictably
  //r0 is nano seconds, r1 is number of nanoseconds per iteration of this function. That was 83ns for a 72MHz M3.
  //caller must pre-tweak value for function call overhead.
Cfunction nanoSpin
  sub r0,r1
  bpl nanoSpin
  bx lr

//include core_atomic.h to use the following atomic accesss routines
// in the atomic_* routines we can't use the ITF instruction mechanism on the clrex instruction, we have to do old fashion branches.

//r0 address, trusted to be 32-bit aligned. r1 is scratched.
Cfunction atomic_increment
  ldrex r1,[r0]
  add r1, r1, #1
  strex r0,r1,[r0]
  bx lr  

//r0 address, trusted to be 32-bit aligned. r1 scratched.
Cfunction atomic_decrement
  ldrex r1,[r0]
  sub r1, r1, #1
  strex r0,r1,[r0]
  bx lr

//r0 address, trusted to be 32-bit aligned. r1 scratched.
Cfunction atomic_decrementNotZero
  ldrex r1,[r0]
  cmp r1, #0
  beq 1f
  sub r1, r1, #1
  strex r0,r1,[r0]
  bx lr
1: //if 0 we still need to remove our lock, and return false
  clrex
  mov r0,r1 //r1 is conveniently the value we need to return.
  bx lr

//r0 address, trusted to be 32-bit aligned. r1 scratched.
Cfunction atomic_incrementNotMax
  ldrex r1,[r0]
  add r1, r1, #1
  cmp r1,#0
  beq 1f
  strex r0,r1,[r0]
  bx lr
1: //if 0 we still need to remove our lock, and return false
  clrex
  mov r0,r1 //r1 is conveniently the value we need to return.
  bx lr

//r0 address, trusted to be 32-bit aligned. r1 new value, r2 scratched.
Cfunction  atomic_setIfZero
  //lock and load
  ldrex r2,[r0]
  //if not zero bail out.
  cmp r2,#0
  bne 1f
  //is zero, write over it.
  strex r0,r1,[r0]
  bx lr
1: //we still need to remove our lock, and return 'success'
  clrex
  mov r0,#0
  bx lr

  //r0 address, r1 increment, r2 scratched
Cfunction atomic_add
  ldrex r2,[r0]
  add r2, r2, r1
  strex r0,r2,[r0]
  bx lr

.end
