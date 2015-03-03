int wtf(int complaint);

extern "C" {
//inlined  void __libc_init_array(void); // The entry point for the C++ library startup
void SystemInit(void); // __USE_CMSIS __USE_LPCOPEN
int main(void); // entry point
void generateHardReset(void); // doesn't return! supplied by nvic.cpp as that is where reset hardware happens to reside.

/** these structs will be created via LONG(...) directives in the ld file.
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

const extern RamInitBlock __data_segment__;//name coordinated with cortexm.ld
const extern RamBlock __bss_segment__;  //name coordinated with cortexm.ld


/** NB: this startup presumes 32 bit aligned, 32bit padded structures, but linker gives byte addresses and counts */
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

/** every type of code driven initialization is invoked through a simple call to a generated routine
that pushes arguments and then calls a normal routine. EG:
 static Classy thing(90120,true); results in
 initClassyThing:
  push true;
  push 90120;
  push &thing;
  call Classy::Classy(int,bool);

  AND an entry into the init_table:
  &initClassyThing,
*/
typedef void (*InitRoutine)(void) ;
const extern InitRoutine __init_table__[];//name coordinated with cortexm.ld

/** this implementation trusts the linker to null terminate the table */
void run_init( const InitRoutine * table){
  while(InitRoutine routine=*table++){
    (*routine)();
  }
}

}//end extern "C".

// instead of tracking #defined symbols just dummy up the optional routines:
__attribute__ ((weak,optimize(3))) void SystemInit(void) {
  wtf(100001);
}

/** sometimes pure virtual functions that aren't overloaded get called anyway,
  * such as from extended classes prophylactically calling the overloaded parent,
    or constructors calling their pure virtual members */
void __cxa_pure_virtual(){  /* upon call of pure virtual function */
  wtf(100000); /* ignore it */
}

__attribute__ ((weak,naked,noreturn,optimize(3))) void generateHardReset(){
  while(1){
    wtf(100002);
  }
}

/*****************************************************************************
 * Reset entry point. Not actually an isr as it is jumped to, not called nor vectored to.
 * Sets up a simple runtime environment and initializes the C/C++ library.
 */
extern "C" void cstartup(void);
__attribute__ ((naked,noreturn)) void cstartup(void){

  // initialize static variables
  data_init(__data_segment__);
  // Zero other static variables.
  bss_init(__bss_segment__);
  // a hook:
  SystemInit(); // stuff that C++ construction might need, like turning on hardware modules (e.g. LPC::GPIO::Init())
  run_init(__init_table__);
  //incorporated by linker into our run_init  __libc_init_array(); // C++ library initialization (? constructors for static objects?)
  main();
  // todo: theoretically could find and execute destructors for static objects.
  generateHardReset(); // auto restart on problems, design your system to tolerate spontaneous power cycles on fatal firmware error
} // start

// stack pointer is set to end of ram via linker script
void (*resetVector)(void) __attribute__((section(".vectors.1"))) = cstartup;
// rest of table is in nvic.cpp, trusting linker script to order files correctly for now. Need to extend the section names for safety.

#ifdef __linux__
const RamInitBlock __data_segment__={0,0,0};//(&__data_segment__,&__data_segment__,0);//name coordinated with cortexm.ld
const RamBlock __bss_segment__={0,0};//(&__bss_segment__,0);  //name coordinated with cortexm.ld
const InitRoutine __init_table__[]={nullptr};//name coordinated with cortexm.ld

#endif
