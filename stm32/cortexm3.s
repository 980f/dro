//most of these are prototyped in minimath.h, really should give them their own header file as only some are relzated to mathmetics.

//declares an asm routine that can be accessed via C without much pain. C prototype will need 'extern "C"' treatment.
.macro Cfunction fname
  .global \fname
  .thumb
  .thumb_func

\fname :
  .endm

  .align 4   //align nanoSpin so that flash cache works predictably
  Cfunction nanoSpin
  //todo:3 rework so that the 83 comes from the clock setting code.
  //the '83' is 6 clocks at 72Mhz. 
  sub r0,#83
  bpl nanoSpin
  bx lr

//this routine will fail if the product of first two operands exceeds 2^32.
  Cfunction muldivide
  umull r0,r1,r0,r1  //in typical use this is u16 stretched to u32 times the same
  lsr r1,r2,#1       //add 1/2 denom, only unsigned denoms are supported
  add r0,r0,r1       //to get a rounded quotient.
  udiv r0,r0,r2      //and now divide by another such stretched u16
  bx lr

  //u16 fractionallyScale(u16 number,u16 fraction,u16 numbits)
  Cfunction fractionallyScale
  umull r0,r1,r0,r1  //in typical use this is u16 stretched to u32 times the same
  //shift ms part right by r2 and return that.
  lsr r0,r2
  bx lr

  Cfunction log2Exponent
  clz r0,r0
  rsb r0,r0,#32
  bx lr

  CFunction shiftScale //float (float eff,int pow2){
  lsl r1,r1,#23
  sub r0,r0,r1
  bx lr

  Cfunction  flog2
  //todo:2 push temps
  mov r1,r0  //r1 is unshifted mantissa
  bl log2Exponent //ro is integer part
  lsr r1,r0  //r1 is ms aligned mantissa
  //todo:2 much more code!
  //cmp r1  ; add #1
  //todo:2 pop temps
  bx lr

  //copyObject(u32 source,u32 target,u32 length)
  Cfunction copyObject
  cmp r0, r1
  beq 2f
  cmp r2,#0
  beq 2f
   //todo:3 if more than say 8 bytes then recognize aligment and use larger moves
1:
  ldrb r3, [r0]
  add r0, r0, #1
  strb r3, [r1]
  add r1, r1, #1
  sub r2, r2, #1
  bne 1b
2:
  bx lr

//C startup uses start to end rather than start and length:
  Cfunction memory_copy
  sub r2, r2, r1  //end(+1)-start=length
  bne copyObject
  bx lr
  
  
  Cfunction memory_set
  cmp r0, r1
  beq 1f
  strb r2, [r0]
  add r0, r0, #1
  b memory_set
1:
  bx lr

  Cfunction  fillObject  //(void* target,u32 length,u8 fill);
  strb r2, [r0]
  add r0, r0, #1
  sub r1, r1, #1
  bne fillObject
  bx lr

