
INCLUDE stm32f103re.memdef

/*
 sectionName logicalAddress : [AT romAddress] {
    _start = .; // start==logicalAddress
 }

So you must move the location counter to the targeted address, but the AT command tracks where in the rom the related data is.
Documentation of expressions inside a section being relative to the section actually means that the section's logical address is
added to the value of any symbol assigned within its braces.
todo: use ALIGN_WITH_INPUT instead of the BLOCK(4)?
For rom stuff:
 sectionName BLOCK(4) :{
  *(names)
 }
 section_start= ADDR(sectionName);// where in target space the block starts
 LOADADDR(section) is where the AT command put it.

*/

SECTIONS {
/* include memory layout definitions file. */

/* interrupt table */
  .vectors ORIGIN(FLASH):{
    KEEP (*( SORT(.vectors.*)  .vectors))
  } >FLASH
/* normal code */
  .text : {
    /* slh commented out segments produced by features we don't want to be using, some not available on cortexM? parts */
    *(.text .text.* /*.glue_7t .glue_7 .gnu.linkonce.t.* .gcc_except_table .ARM.extab* .gnu.linkonce.armextab.*  */)
  } >FLASH

/* atstartup()? */
  .init : {
    KEEP (*(.init .init.*));
  } >FLASH

 /* static constructors, and something called init_array which seems to have the same syntax as static construction. */
  .ctors :{
    KEEP (*(SORT(.ctors.*))) KEEP (*(.ctors)) KEEP (*(.init_array)) KEEP (*(SORT(.init_array.*)))
  } >FLASH
/* fully const'ed data, note that some things declared const end up in .data despite valiant attempts to compile time compute values. */
  .rodata :{
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  } >FLASH
/* we have ended the purely rom stuff, to follow is the ram initialization table.*/

  packer = ALIGN(4); /* must follow last purely RO section */

  /*figure out how much data we need*/
  /* experiment exceeds trail license ;)
  sizer RAMSTART : {
    *(.data .data.* .gnu.linkonce.d.*)
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __stack_size__ = LENGTH(SRAM) - SIZEOF(sizer);

    . +=__stack_size__;
    */
/* want data initialization rom source to be aligned for faster copies */

  .data : /*AT(packer)*/{
    *(.data .data.* .gnu.linkonce.d.*)
  } >SRAM AT>FLASH

/* NOLOAD is gratuitous, but is a marker that all that is occurring is data allocation */
  .bss (NOLOAD) : {
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  } >SRAM

  __stack_end__ = .;  /* if SP is ever below this value we are corrupting memory. */
  __stack_max__ = LENGTH(SRAM) - (__stack_end__ -ORIGIN(SRAM)); /* value to feed into start of data segment */

/* the following symbol definitions are referencable by code, the linker puts there values into the text when writing out the files */

  __data_target__ = ADDR(.data);
  __data_source__ = LOADADDR(.data);
  __data_length__ = SIZEOF(.data);

  __zero_start__ = ADDR(.bss);
  __zero_length__ = SIZEOF(.bss);


  __init_start__ = ADDR(.init);
  __init_length__ = SIZEOF(.init);

  __ctors_start__ = ADDR(.ctors);
  __ctors_length__ = SIZEOF(.ctors);

}

