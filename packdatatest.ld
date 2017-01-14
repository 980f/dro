/* first include your processor's MEMORY layout stuff.*/
/* inlined lpc1343 values, normally I INCLUDE <processorName>.ld  */
MEMORY {
  UNPLACED_SECTIONS (wx) : ORIGIN = 0xFFFFFFF0, LENGTH = 0
  SRAM (wx) : ORIGIN = 0x10000000 , LENGTH = 8K
  FLASH (rx) : ORIGIN = 0, LENGTH = 32K
}


/* then the generic "flash only" stuff, which presently is tightly tied into cstartup.cpp */
INCLUDE cortexm/cortexm.ld


