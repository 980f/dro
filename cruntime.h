#pragma once

#define CRUNTIME_H
/** parts of the c startup that might be useful to a running program */

extern "C" [[gnu::naked,noreturn]] void generateHardReset();

/** these structs are created via LONG(...) directives in the ld file.
   That way only one symbol needs to be shared between the ld and this file for each block.*/
struct RamBlock {
  unsigned int *address;
  unsigned int length;
  /** NB: this startup presumes 32 bit aligned, 32bit padded bss, but linker gives byte addresses and counts.
   *  Does anyone remember what BSS originally meant? Nowadays it is 'zeroed static variables' */
  void go(void){
    length /= sizeof(unsigned int); // convert byte count to u32 count.
    while(length-- > 0) {
      *address++ = 0;
    }
  }
};

struct RamInitBlock {
  const unsigned int *rom;
  RamBlock ram;
  /** NB: this startup presumes 32 bit aligned, 32bit padded structures, but linker gives byte addresses and counts */
  void go(void){
    ram.length /= sizeof(unsigned int); // convert byte count to u32 count.
    while(ram.length-- > 0) {
      *ram.address++ = *rom++;
    }
  }
};

/** every type of code driven initialization is invoked through a simple void fn(void)*/
typedef void (*InitRoutine)(void) ;
/** this implementation trusts the linker script to null terminate the table.
 * To insert something into the table you must put it into a section .ctors[.*]. lower number/alpha after the 'ctors' run earlier.
 * the 'initialization priority logic' uses such sections to control startup sequencing, and doesn't work without linker support.
 */
void run_init( const InitRoutine * table);

/** generate hard reset if the stack has overflowed */
void stackFault();
