#include "cruntime.h" //to validate it, herein lay default implementations of it.
#include "wtf.h"  //error routine, a place to share a breakpoint for trouble.

/*CMSIS uses this function for doing things that must precede C init, such as turning on IO ports so that configuration commands in constructors function.
You can't use any static *object* but you can use static initialized plain old data */
void SystemInit(void); // __USE_CMSIS __USE_LPCOPEN

#pragma GCC diagnostic ignored "-Wmain"
int main(void); // entry point

/* my approach to dealing with errors is to start over. Once I've restored 'atExit' functionality you can do a while(1) there to prevent a restart */
extern void generateHardReset(void); // doesn't return! supplied by nvic.cpp as that is where reset hardware happens to reside.

//the linker script creates and initializes these constant structures, used by cstartup() to initialize ram:
//non-trivially initialized POD.
const extern RamInitBlock __data_segment__;//name coordinated with cortexm.ld
//zero initialized ram
const extern RamBlock __bss_segment__;  //name coordinated with cortexm.ld
//run all code in .init sections.
const extern InitRoutine __init_table__[];//name coordinated with cortexm.ld

/** NB: this startup presumes 32 bit aligned, 32bit padded structures, but linker gives byte addresses and counts. 
OTOH gnu c compiler puts alignment commands in every section so we rely upon that here for speed. */
void data_init(const RamInitBlock &block) /*ISRISH*/ {
  RamInitBlock copy=block;
  copy.ram.length /= sizeof(unsigned int); // convert byte count to u32 count.
  while(copy.ram.length-- > 0) {
    *copy.ram.address++ = *copy.rom++;
  }
}

/** NB: this startup presumes 32 bit aligned, 32bit padded bss, but linker gives byte addresses and counts.
 *  Does anyone remember what BSS originally meant? Nowadays it is 'zeroed static variables' */
void bss_init(const RamBlock &block)  {
  RamBlock bss=block;
  bss.length /= sizeof(unsigned int); // convert byte count to u32 count.
  while(bss.length-- > 0) {
    *bss.address++ = 0;
  }
}

/** every type of code driven initialization is invoked through a simple void fn(void)*/
typedef void (*InitRoutine)(void) ;
const extern InitRoutine __init_table__[];//name coordinated with cortexm.ld

/** this implementation trusts the linker script to null terminate the table */
void run_init( const InitRoutine * table){
  while(InitRoutine routine=*table++){
    (*routine)();
  }
}

// instead of tracking #defined symbols just dummy up the optional routines:
[[gnu::weak,gnu::optimize(3)]] void SystemInit(void) {
  return;
}

/** sometimes pure virtual functions that aren't overloaded get called anyway,
  * such as from extended classes prophylactically calling the overloaded parent,
    or constructors calling their pure virtual members */
extern "C" void __cxa_pure_virtual(){  /* upon call of pure virtual function */
  wtf(100000); /* ignore it, but have a place for a breakpoint */
}

/** Reset entry point. The chip itself has set up the stack pointer to top of ram, and then the PC to this. It has not set up a frame pointer.
 */
extern "C" //to make it easy to pass this to linker sanity checker.
[[gnu::naked,noreturn]] //we don't need no stinking stack frame (no params, no locals)
void cstartup(void){
  // initialize static variables
  data_init(__data_segment__);
  // Zero other static variables.
  bss_init(__bss_segment__);
  // a CMSIS hook:
  SystemInit(); // stuff that C++ construction might need, like turning on hardware modules (e.g. my LPC::GPIO::Init())

  run_init(__init_table__);
  //incorporated by linker into our run_init:  __libc_init_array(); // C++ library initialization (includes constructors for static objects)
  main();
  // todo: find and execute destructors for static objects and do atexit routines.
  generateHardReset(); // auto restart on problems, design your system to tolerate spontaneous power cycles on fatal firmware error
}

// stack pointer is set to end of ram via linker script
void (*resetVector)(void) __attribute__((section(".vectors.1"))) = cstartup;
// rest of table is in nvic.cpp, trusting linker script to order files correctly per the numerical suffix

extern unsigned const __stack_limit__;//created in linker script
void stackFault(){
  unsigned here;
  if(&here <= &__stack_limit__){//todo: should add a few so that we can call wtf without risk
    wtf(99999999);
    generateHardReset();
  }
}

#ifdef __linux__  //for testing compiles with PC compiler etc.
const RamInitBlock __data_segment__={0,{0,0}};
const RamBlock __bss_segment__={0,0};
const InitRoutine __init_table__[]={nullptr};
const unsigned * __stack_end__(0);
#else
//destructor failure stuff, but ours won't ever actually fail
extern "C" int __aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle){
  return 0;
}

//extern "C" {
//  void __aeabi_atexit(){
//    wtf(-1);
//  }
//  void __dso_handle(){
//    wtf(-2);
//  }
//}
#endif
