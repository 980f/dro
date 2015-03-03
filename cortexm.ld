/*
simplified linker script for bare metal flash execution only

This should be included *after* a script that defines MEMORY sections SRAM and FLASH.

This takes advantage of gcc emitting data alignment commands in the input files, so we don't have to redundantly claim them herein.

For the c startup initialization it trades off number of inline constants (register load immediates) with
putting those constants explicitly in the rom and inlining only a pointer thereto.

The building of the vector table is shared between linker generated stack top and the vector table proper being
declared elsewhere (@see alh's nvic.cpp) using the same facility that 'init' attributes uses.

Some people (CMSIS- looking at you) conditionally include some special calls around the c startup (useful for things like powering up parts of the
chip that need to be on before c-structs for accessing them are constructed), I have implemented that via weak references
to a routine that devolves into a "bx lr".

Some people have multiple initialization call tables, I pack them into one segment and null terminate it
which eliminates the need for a slew of shared symbols and counting. I may have unintentionaly altered the
priority, the point of the multiple lists was to allow for some control over order in which initialization
routines execute, which is done via the order of input declarations.

No special provision has been made for a ram vector table. The compiler can align a user built array of function pointers
and user code can assign to the vector table system control register to enable it.

I have left off destructor and other 'atexit' functionality, my only 'atexit' is to generate a hard reset.

I have left off a no-init data section, that is a dangerous technology with many different possible implementations.
The stm32 non-volatile data has holes in it beyond the ability of the linker to readily compensate for. Not zeroing some ram
that will programmaticallly be loaded from a non-volatile device is a minor performance burden for keeping this script generic.

I have made it convenient to swap around the order of the sections without having to rework explict references to the others.

*/

SECTIONS {
/* interrupt table */
  .vectors ORIGIN(FLASH):{
    LONG( ORIGIN(SRAM)+LENGTH(SRAM))  /* set initial stack pointer to top of sram */
    KEEP (*( SORT(.vectors.*)  .vectors)) /* followed by reset and other handlers */
  } >FLASH

/* normal code */
  .text : {
    *(.text .text.*)
     /* alh commented out segments produced by features he doesn't want to be using, some of which are not available on cortexM? parts */
     /**(.glue_7t .glue_7 .gnu.linkonce.t.* .gcc_except_table .ARM.extab* .gnu.linkonce.armextab.*)  */
  } >FLASH

/* things that should be called before main():*/
  .init : {
/* code that initializes static data via run-time computation.*/
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
 /* static constructors after C lib inits*/
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    LONG(0)  /*mark end of table, instead of having code for decrementing a count */
  } >FLASH
  __init_table__ = LOADADDR(.init);

/* fully const'ed data, note that some things declared const end up in .data despite valiant attempts to compile time compute values. */
  .rodata :{
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  } >FLASH

/* zero initialized data */
  .bss (NOLOAD) : {/* NOLOAD is gratuitous, but is a reminder that all that is occurring is data allocation.*/
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  } >SRAM
  .startupClear : {/* the following initializes what my cstartup.cpp calls 'struct RamBlock' */
    LONG(ADDR(.bss))
    LONG(SIZEOF(.bss))
  } >FLASH
  __bss_segment__ = LOADADDR(.startupClear);

/* data with constant initializers */
  .data :{
    *(.data .data.* .gnu.linkonce.d.*)
  } >SRAM AT>FLASH
  .startupCopy : { /* this initializes what my cstartup.cpp calls 'struct RamInitBlock' */
    LONG(LOADADDR(.data))
    LONG(ADDR(.data))
    LONG(SIZEOF(.data))
  } >FLASH
  __data_segment__ = LOADADDR(.startupCopy);

/* diagnostic, or feed into a runtime stack check routine */

  .findEnd :{ /* cute trick to get the total SRAM used. This trick keeps us from having to move the __stack_end__ assignment into whichever ram block becomes the last.*/
    __stack_end__ = .;  /* if SP is ever below this value we are corrupting memory. */
  } >SRAM
  /* value to someday feed into start of data segment should we ever figure out how to put the stack first so that we get a fault instead of wild behavior */
  __stack_max__ = LENGTH(SRAM) - (__stack_end__ - ORIGIN(SRAM)); 
}

