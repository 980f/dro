#ifndef MEMDEF_H
#define MEMDEF_H 1

#if 0 //MCU == "lpc1343"
#define SRAM_BASE 256*1024*1024
#define SRAM_K    8
#endif

#if 1 // MCU == "stm32f103"
#define SRAM_BASE 512*1024*1024
#define SRAM_K    32
#endif

#endif