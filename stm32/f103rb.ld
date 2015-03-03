/*
for stm32F1xx we have a choice between 128M and 0 for rom start.
128M always works, 0 only if you don't boot to ram.
0 makes the map easier to read.
*/

MEMORY {
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  SRAM (wx) : ORIGIN = 512M , LENGTH = 20K
  FLASH (rx) : ORIGIN = 128M, LENGTH = 128K
}
