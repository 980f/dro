#pragma once

#define CRUNTIME_H
/** parts of the c startup that might be useful to a running program */

extern "C" void generateHardReset();

/** these structs are created via LONG(...) directives in the ld file.
   That way only one symbol needs to be shared between the ld and this file for each block,
   and the machine code reduction makes up for the explicit storing of these values in the rom .*/
struct RamBlock {
  unsigned int *address;
  unsigned int length;
};

struct RamInitBlock {
  const unsigned int *rom;
  RamBlock ram;
};

/** NB: this startup presumes 32 bit aligned, 32bit padded structures, but linker gives byte addresses and counts */
void data_init(const RamInitBlock &block);

/** NB: this startup presumes 32 bit aligned, 32bit padded bss, but linker gives byte addresses and counts.
 *  Does anyone remember what BSS originally meant? Nowadays it is 'zeroed static variables' */
void bss_init(const RamBlock &block);

/** every type of code driven initialization is invoked through a simple void fn(void)*/
typedef void (*InitRoutine)(void) ;
/** this implementation trusts the linker script to null terminate the table */
void run_init( const InitRoutine * table);

/** generate hard reset if the stack has overflowed */
void stackFault();
