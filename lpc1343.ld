/*
for stm32F1xx we have a choice between 128M and 0 for rom start.
128M always works, 0 only if you don't boot to ram.
*/

MEMORY {
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  CM3_System_Control_Space (wx) : ORIGIN = 0xe000e000, LENGTH = 4K
  SRAM (wx) : ORIGIN = 0x10000000 , LENGTH = 8K
  FLASH (rx) : ORIGIN = 0, LENGTH = 32K
}
