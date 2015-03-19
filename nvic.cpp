/*all about interrupts*/
#include "nvic.h"
#include "eztypes.h"
#include "peripheraltypes.h"
#include "cruntime.h"

volatile int CriticalSection::nesting = 0;
/////////////////////////////////

u8 IrqAccess::setPriority(u8 newvalue) const { // one byte each, often only some high bits are implemented
  u8 &priorityRegister(*reinterpret_cast<u8 *>(0xE000E400 + number));
  u8 oldvalue = priorityRegister;

  priorityRegister = newvalue;
  return oldvalue;
}

/////////////////////////////////

void Irq::enable(){
  if(locker > 0) { // if locked then reduce the lock such that the unlock will cause an enable
    --locker;  // one level earlier than it would have. This might be surprising so an
    // unmatched unlock might be the best enable.
  }
  if(locker == 0) { // if not locked then actually enable
    IrqAccess::enable();
  }
}

void Irq::prepare(){
  clear(); // acknowledge to hardware
  enable(); // allow again
}

/////////////////////////////////

IRQLock::IRQLock(Irq &irq, bool inIrq): irq(irq){
  if(! inIrq) {
    irq.lock();
  }
}

IRQLock::~IRQLock(){
  irq.enable();
}

/////////////////////////////////

struct InterruptController {
  // ICSR
  volatile unsigned int active : 9; // isr we are within
  unsigned int : 2;
  volatile unsigned int unnested : 1; // status: will return to "not-interrupted" if isr ends now.
  volatile unsigned int pending : 9; // highest priority one pending, if>active we are about to nest :)
  volatile unsigned int isrpending : 1; // non-NMI, non-Fault is pending
  volatile unsigned int isrpreempt : 1; // about to be nested, only useful for debug.
  unsigned int : 1;
  unsigned int pendTickClr : 1;
  unsigned int pendTickSet : 1;
  unsigned int pendSVClr : 1;
  unsigned int pendSVSet : 1;
  unsigned int : 2;
  unsigned int pendNMISet : 1;
  u32 VectorTableBase; // bits 6..0 better be set to 0!

  struct AIRC {
    unsigned int hardReset : 1; // generate a hardware reset, maybe
    unsigned int eraseIrqState : 1; // abend interrupt handling activity, but doesn't fixup the stack
    unsigned int pleaseReset : 1; // will generate a hardware reset
    unsigned int priorityGrouper : 3; // indirectly the number of interrupt levels
    unsigned int endianNess : 1; // read only
    u16 VectorKey; // write 05FA to write to any of the above fields.
  };

  u32 airc; // must build an object then copy as a u32 to this field.

  unsigned int : 1;
  unsigned int sleepOnExit : 1; //
  unsigned int sleepDeep : 1;
  unsigned int : 1;
  unsigned int sevOnPend : 1; // let interrupts be events for WFE
  unsigned int : 32 - 5;

  // u32 CCR;
  unsigned int allowThreadOnReturn : 1; // only an RTOS would want this available
  unsigned int allowSoftInterruptTriggers : 1; // allow user code to simulate interrupts
  unsigned int : 1;
  unsigned int trapUnaligned : 1;
  unsigned int trapDivBy0 : 1; // a heinous thing to do
  unsigned int : 3;
  unsigned int ignoreMisalignmentInFaultHandlers : 1;
  unsigned int : 1; // stkalign, let hardware manage this
  unsigned int : 32 - 10;

  u8 priority[12]; // syscall settable prorities, 4 .. 15, -12 to -1 in our unified numbering
  // volatile u32 SHCSR;
  volatile unsigned int memFault : 1;
  volatile unsigned int busFault : 1;
  unsigned int : 1;
  volatile unsigned int usageFault : 1;
  unsigned int : 3;
  volatile unsigned int serviceCall : 1;
  volatile unsigned int monitorActive : 1;
  unsigned int : 1;
  volatile unsigned int serviceFlagActive : 1;
  volatile unsigned int sysTicked : 1;
  volatile unsigned int usageFaultPending : 1;
  volatile unsigned int memFaultPending : 1;
  volatile unsigned int busFaultPending : 1;
  volatile unsigned int serviceCallPending : 1;

  unsigned int memoryFaultEnable : 1;
  unsigned int busFaultEnable : 1;
  unsigned int usageFaultEnable : 1;
  unsigned int : 32 - 19;

  u8 memFaultInfo;
  u8 busFaultInfo;
  u16 usageFaultInfo;

  u32 hardFaultStatus;
  u32 debugFaultStatus;
  u32 mmFaultAddress;
  u32 busFaultAddress;
  volatile u32 AFSR;
  // cpuid table
  // coprocessor
};

// 0xE000EF00: write an interrupt id number to get it to pend.
// nvic[INT_ENA]=1
// basepri precludes higher numbered interrupts from occuring.
// primask precludes any normal interrupt
// faultmask precludes most faults.
/*
 * void __set_PRIMASK (uint32_t value)  M0, M3  PRIMASK = value  Assign value to Priority Mask Register (using the instruction MSR)
 * uint32_t __get_PRIMASK (void)  M0, M3  return PRIMASK  Return Priority Mask Register (using the instruction MRS)
 * void __enable_fault_irq (void)  M3  FAULTMASK = 0  Global Fault exception and Interrupt enable (using the instruction CPSIE f)
 * void __disable_fault_irq (void)  M3  FAULTMASK = 1  Global Fault exception and Interrupt disable (using the instruction CPSID f)
 */
soliton(InterruptController, 0xE000ED04);

void configurePriorityGrouping(int code){
  atAddress(0xE000ED0C) = ((code & 7) << 8) | 0x05FA0000; //5FA is a guard against random writes.
}

extern "C" { // to keep names simple for "alias" processor
  void unhandledFault(void){
    register int num = theInterruptController.active;

    if(num >= 4) {
      theInterruptController.priority[num - 4] = 0xFF; // lower them as much as possible
    }
    switch(num) {
    case 0: // surreal: stack pointer init rather than an interrupt
    case 1: // reset
      // todo:3 reset vector table base to rom.
      break;
    case 2: // NMI
      // nothing to do, but pin doesn't exist on chip of interest to me
      break;
    case 3: // hard Fault
      /** infinite recursion gets here, stack trashing, I've had vptr's go bad...*/
      generateHardReset(); // since we usually get into an infinite loop.
      /* used hard reset rather than soft as my hardware module interfaces expect it.*/
      break;
    case 4: // memmanage
      theInterruptController.memoryFaultEnable = 0;
      break;
    case 5: // bus
      theInterruptController.busFaultEnable = 0;
      break;
    case 6: // usage
      theInterruptController.usageFaultEnable = 0;
      break;
    case 7: // nothing
    case 8: // nothing
    case 9: // nothing
    case 10: // nothing
    case 11: // sv call
      // do nothing
      break;
    case 12: // debug mon
    case 13: // none
    case 14: // pend SV (service requested by bit set rather than instruction
      break;
    case 15: // systick
      // todo:2 disable systick interrupts
      break;
    } /* switch */
  } /* unhandledFault */


/* turn it off so it doesn't happen again, and a handy breakpoint */
  void unhandledInterruptHandler(void){
    int irqnum = theInterruptController.active - 16;

    Irq(irqnum).disable();
  } /* unhandledInterruptHandler */
} // end extern "C"

// the stubs declare handler routines that deFault to unhandledInterruptHandler or unhandledFault if not otherwise declared.

#define stub(irq) void IRQ ## irq(void) __attribute__((weak, alias("unhandledInterruptHandler")))

#define stubFault(index) void FAULT ## index(void) __attribute__((weak, alias("unhandledFault")))

typedef void (*Handler)(void);
stubFault(0);
stubFault(1);
stubFault(2);
stubFault(3);
stubFault(4);
stubFault(5);
stubFault(6);
stubFault(7);
stubFault(8);
stubFault(9);
stubFault(10);
stubFault(11);
stubFault(12);
stubFault(13);
stubFault(14);
stubFault(15);

stub(0);
stub(1);
stub(2);
stub(3);
stub(4);
stub(5);
stub(6);
stub(7);
stub(8);
stub(9);
stub(10);
stub(11);
stub(12);
stub(13);
stub(14);
stub(15);
stub(16);
stub(17);
stub(18);
stub(19);
stub(20);
stub(21);
stub(22);
stub(23);
stub(24);
stub(25);
stub(26);
stub(27);
stub(28);
stub(29);
stub(30);
stub(31);
stub(32);
stub(33);
stub(34);
stub(35);
stub(36);
stub(37);
stub(38);
stub(39);
stub(40);
stub(41);
stub(42);
stub(43);
stub(44);
stub(45);
stub(46);
stub(47);
stub(48);
stub(49);
stub(50);
stub(51);
stub(52);
stub(53);
stub(54);
stub(55);
stub(56);
stub(57);
stub(58);
stub(59);
// todo:3 device model specific number of these

Handler VectorTable[] __attribute__((section(".vectors.2"))) = {//0 is stack top, 1 is reset vector, 2 for the rest.
  FaultName(2),
  FaultName(3),
  FaultName(4),
  FaultName(5),
  FaultName(6),
  FaultName(7), //NB: the lpc guys have a tendency to shove a checksum here.
  FaultName(8),
  FaultName(9),
  FaultName(10),
  FaultName(11),
  FaultName(12),
  FaultName(13),
  FaultName(14),
  FaultName(15),

  IrqName(0),
  IrqName(1),
  IrqName(2),
  IrqName(3),
  IrqName(4),
  IrqName(5),
  IrqName(6),
  IrqName(7),
  IrqName(8),
  IrqName(9),
  IrqName(10),
  IrqName(11),
  IrqName(12),
  IrqName(13),
  IrqName(14),
  IrqName(15),
  IrqName(16),
  IrqName(17),
  IrqName(18),
  IrqName(19),
  IrqName(20),
  IrqName(21),
  IrqName(22),
  IrqName(23),
  IrqName(24),
  IrqName(25),
  IrqName(26),
  IrqName(27),
  IrqName(28),
  IrqName(29),
  IrqName(30),
  IrqName(31),
  IrqName(32),
  IrqName(33),
  IrqName(34),
  IrqName(35),
  IrqName(36),
  IrqName(37),
  IrqName(38),
  IrqName(39),
  IrqName(40),
  IrqName(41),
  IrqName(42),
  IrqName(43),
  IrqName(44),
  IrqName(45),
  IrqName(46),
  IrqName(47),
  IrqName(48),
  IrqName(49),
  IrqName(50),
  IrqName(51),
  IrqName(52),
  IrqName(53),
  IrqName(54),
  IrqName(55),
  IrqName(56),
  IrqName(57),
  IrqName(58),
  IrqName(59),
  // todo:3 device model specific quantity.
};

#if 0
//asm code:
.global generateHardReset
.thumb
.align 2
.thumb_func

generateHardReset:
//AIRC register
movw r0, #0xED0C
movt r0, #0xE000
//1:VECTRESET worked, 4:SYSRESETREQ just loops here, using rowley&jtag debugger. //probably should try 5 in case different vendors misread the arm spec differently.
movw r1, #5
movt r1, #0x05FA
str r1,[r0]
b generateHardReset
#endif

__attribute__((naked,noreturn)) //trying to get good assembler code on this one :)
void generateHardReset(){
  do {//keep on hitting the bit until we reset.
    theInterruptController.airc=0x5FA0005;//1 worked on stm32, 4 should have worked but looped under the debugger.
  } while (1);
}
