#include "uart.h"
#include "lpcperipheral.h" // the peripheral is wholly hidden within this module.
#include "gpio.h" // to gain control of pins
#include "clocks.h"
#include "syscon.h" // to enable clock
#include "bitbanger.h" // for BitField
#include "nvic.h"  // for isr
using namespace LPC;

// apb dev 2
// interrupt
/** 0=disabled else divide by 1:255.
 *  Could use a simple SFR, but would still want a function to validate the range. */
SFRfield<sysConReg(0x98), 0, 8> uartClockDivider;

// irq num must be define, used by simple macros.
#define uartIrq 46

Irq uirq(uartIrq);

Uart theUart;
// vector isn't declared here as we need the concrete class.
// if we use function pointers instead of extension then we can do it here.
ObjectInterrupt(theUart.isr(), uartIrq);

namespace LPC {
/*------------- Universal Asynchronous Receiver Transmitter (UART) 16550 equivalent type -----------*/
struct UART550 {
  /** actually overlapped registers. For some stupid reason they are maintaining compatibility with code that would have been written for other processors. SO that is cute but they could have also provided direct access so that the aggravating overlay of registers isn't necessary.*/
  union {
    /** read receive fifo */
    const SFR RBR;
    /** data to transmit fifo */
    SFR THR;
    /** baud rate divisor lower word */
    SFR DLL;
  };

  union {
    /** baud rate divisor upper word */
    SFR DLM;
    /** interrupt enable */
    SFR IER;
  };

  union {
    const SFR IIR;
    SFR FCR;
  };
  SFR LCR;
  SFR MCR;
  const SFR LSR;
  const SFR MSR;
  SFR SCR;
  SFR ACR;
  SFR ICR;
  SFR FDR;
  SKIPPED RESERVED0;
  SFR TER;
  SKIPPED RESERVED1[6];
  SFR RS485CTRL;
  SFR ADRMATCH;
  SFR RS485DLY;
  const SFR FIFOLVL;
};
} // namespace LPC

DefineSingle(UART550, apb0Device(2));
// going through one level of computation in expectation that we will meet a part with more than one uart:
constexpr unsigned uartRegister(unsigned offset){
  return apb0Device(2) + offset;
}

// line control register:
constexpr unsigned LCR = uartRegister(0x0c);
/** number of bit, minus 5*/
SFRfield<LCR, 0, 2> numbitsSub5;
SFRbit<LCR, 2> longStop;
/** only 5 relevent values, only 3 common ones*/
SFRfield<LCR, 3, 3> parity;
/** sends break for as long as this is true */
SFRbit<LCR, 6> sendBreak;
/** the heinous divisor latch access bit. */
SFRbit<LCR, 7> dlab;

//interupt enable regsiter:
constexpr unsigned IER=uartRegister(0x04);
SFRbit<IER,0> receiveDataInterruptEnable;
SFRbit<IER,1> transmitHoldingRegisterEmptyInterruptEnable;
SFRbit<IER,2> lineStatusInterruptEnable;
SFRbit<IER,8> AutoBaudCompleteInterruptEnable;
SFRbit<IER,9> AutoBaudTimeoutInterruptEnable;

/** iopin pattern for uart pins: */
constexpr unsigned pickUart = 0b11010001; // rtfm, not worth making syntax

Uart::Uart(): receive(nullptr), send(nullptr){
  uirq.disable();
  GpioPin<PortNumber(1), BitNumber(6)> rxd(pickUart);
  GpioPin<PortNumber(1), BitNumber(7)> txd(pickUart);
  // the 134x parts are picky about order here, the clock must be OFF when configuring the pins.

  /* Enable UART clock */
  ClockController<12>(1); //
  uartClockDivider = unsigned(1); // a functioning value, that allows for the greatest precision, if in range.
}

///** @param which 0:dsr, 1:dcd, 2:ri @param onP3 true: port 3 else port 2 */
// void configureModemWire(int which, bool onP3){
//  //and here we finally find a reason for a dynamically picked iopin:
//  //SFRbit<ioconRegister(0xb4+(which<<2),0)> HSport(onP3);
//  //and until that is finished the user must independently configure the related pin.
// }


unsigned Uart::setBaudPieces(unsigned divider, unsigned mul, unsigned div, unsigned sysFreq) const {
  if(sysFreq == 0) { // then it is a request to use the active value
    sysFreq = coreHz();
  }
  if(mul == 0 || mul > 15 || div > mul) { // invalid, so set to disabling values:
    mul = 1;
    div = 0;
  } else if(div == 0) {
    mul = 1; // for sake of frequency computation
  }
  constexpr unsigned FDR = uartRegister(0x28);
  SFRfield<FDR, 0, 4> fdiv(div);
  SFRfield<FDR, 4, 4> fmul(mul);

  dlab = 1;
  SFRbyte<uartRegister(0x4)> DLM(divider >> 8);
  SFRbyte<uartRegister(0x0)> DLL(divider);
  dlab = 0;
  return /*ratio*/ (mul * sysFreq) / ((mul + div) * divider * uartClockDivider * 16);
} // Uart::setBaud


unsigned Uart::setBaud(unsigned hertz, unsigned sysFreq) const {
  if(sysFreq == 0) {
    sysFreq = coreHz();
  }
  hertz *= 16; // we need 16 times the desired baudrate.

  unsigned pclk;
  unsigned divider;
  unsigned overflow;

  do {
    pclk = sysFreq / uartClockDivider;
    divider = pclk / hertz;
    overflow = divider >> 16;
    uartClockDivider = uartClockDivider + overflow; // not a true variable.
  } while(overflow);
  // maydo: if divider is >64k hit uartClockDivider to bring it into range.
  unsigned error = pclk % hertz;
  if(error > hertz / 2) {
    ++divider;
  }
  // todo: find best pair of 4 bit mul/div to match error/hertz instead of just rounding to nearest.
  return setBaudPieces(divider, 0, 0, sysFreq);
} // Uart::setBaud

void Uart::setFraming(const char *coded) const {
  int numbits = *coded++ - '0';

  if(numbits < 5) {
    return;
  }
  if(numbits > 8) {
    return;
  }
  numbitsSub5 = numbits - 5;
  switch(*coded++) {
  default:
  case 0: return;

  case 'N':
    parity = 0;
    break;
  case 'O':
    parity = 0b001;
    break;
  case 'E':
    parity = 0b011;
    break;
  case 'M':
    parity = 0b101;
    break;
  case 'S':
    parity = 0b111;
    break;
  } // switch
  int stopbits = *coded++ - '0';
  if(stopbits < 1) {
    return;
  }
  if(stopbits > 2) {
    return;
  }
  longStop = stopbits != 1;
} // Uart::setFraming

void Uart::beTransmitting(bool enabled){
  if(enabled){
    if(int nextch = (*send)() < 0) {
      //do nothing, might still be sending from the fifo
    } else {
      theUART550.THR = nextch;
      //enable interrupts
    }
  } else {

  }
}

void Uart::reception(bool enabled){
  receiveDataInterruptEnable=enabled;
  //how bout line status interrupts? .. yeah add those:
  lineStatusInterruptEnable=enabled;
  //note: not our responsiblity to enable in the NVIC, that normally should be left alone during operation.
}

//inner loop of sucking down the read fifo.
unsigned Uart::tryInput(unsigned LSRValue){
  typedef BitWad<7, 0> bits; // look just at the 'OR of 'some corruption' and the 'data available' bits
  for( ; bits::exactly(LSRValue, 1); LSRValue = theUART550.LSR) {
    (*receive)(theUART550.RBR);
  }
  return LSRValue;
}


void Uart::isr(){
  unsigned IIRValue = theUART550.IIR; // read once, so many of these registers have side effects let us practice on this one.

  BitField<1, 3> irqID(IIRValue);

  switch(irqID) {
  case 0: // modem
    break; // no formal reaction to modem line change.
  case 1:  // thre
    if(int nextch = (*send)() < 0) {
      // todo: stop xmit interrupts if fifo empty.
    } else {
      theUART550.THR = nextch;
    }
    break;
  case 2: // rda
    tryInput(1);
    break;
  case 3: { // line error
    unsigned LSRValue = tryInput(theUART550.LSR); // copying other people here, e.g. this deals with overrun error

    if(BitWad<7, 1, 2, 3, 4>::any(LSRValue)) { // someone else's code. grr, bundles OE with the others which is NOT so good.
      (*receive)(~LSRValue); // inform user code
      LSRValue = theUART550.RBR;  // Dummy read on RX to clear cause interrupt
      return;
    }
  }
    break;
  case 4: // reserved
    break;
  case 5: // reserved
    break;

  case 6: // char timeout (dribble in fifo)
    tryInput(1);
    break;
  case 7:
    break;
  } // switch
} // Uart::isr

#if 0

/**************************************************************************/
void uartSendByte (uint8_t byte){
  /* THRE status, contain valid data */
  while(! (theUART.LSR & theUART.LSR_THRE)) {
  }
  theUART.THR = byte;
}

void uartInit(uint32_t baudrate){
  uint32_t fDiv;
  uint32_t regVal;

  NVIC_DisableIRQ(UART_IRQn);

  // Clear protocol control blocks
  memset(&pcb, 0, sizeof(uart_pcb_t));
  pcb.pending_tx_data = 0;
  uartRxBufferInit();

  /* Set 1.6 UART RXD */
  IOCON_PIO1_6 &= ~IOCON_PIO1_6_FUNC_MASK;
  IOCON_PIO1_6 |= IOCON_PIO1_6_FUNC_UART_RXD;

  /* Set 1.7 UART TXD */
  IOCON_PIO1_7 &= ~IOCON_PIO1_7_FUNC_MASK;
  IOCON_PIO1_7 |= IOCON_PIO1_7_FUNC_UART_TXD;

  /* Enable UART clock */
  SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_UART);
  SCB_UARTCLKDIV = SCB_UARTCLKDIV_DIV1;     /* divided by 1 */

  /* 8 bits, no Parity, 1 Stop bit */
  theUART.LCR = (theUART.LCR_Word_Length_Select_8Chars |
                 theUART.LCR_Stop_Bit_Select_1Bits |
                 theUART.LCR_Parity_Disabled |
                 theUART.LCR_Parity_Select_OddParity |
                 theUART.LCR_Break_Control_Disabled |
                 theUART.LCR_Divisor_Latch_Access_Enabled);

  /* Baud rate */
  regVal = SCB_UARTCLKDIV;
  fDiv = (((CFG_CPU_CCLK * SCB_SYSAHBCLKDIV) / regVal) / 16) / baudrate;

  theUART.DLM = fDiv / 256;
  theUART.DLL = fDiv % 256;

  /* Set DLAB back to 0 */
  theUART.LCR = (theUART.LCR_Word_Length_Select_8Chars |
                 theUART.LCR_Stop_Bit_Select_1Bits |
                 theUART.LCR_Parity_Disabled |
                 theUART.LCR_Parity_Select_OddParity |
                 theUART.LCR_Break_Control_Disabled |
                 theUART.LCR_Divisor_Latch_Access_Disabled);

  /* Enable and reset TX and RX FIFO. */
  theUART.FCR = (theUART.FCR_FIFO_Enabled |
                 theUART.FCR_Rx_FIFO_Reset |
                 theUART.FCR_Tx_FIFO_Reset);

  /* Read to clear the line status. */
  regVal = theUART.LSR;

  /* Ensure a clean start, no data in either TX or RX FIFO. */
  while((theUART.LSR & (theUART.LSR_THRE | theUART.LSR_TEMT)) != (theUART.LSR_THRE | theUART.LSR_TEMT)) {
  }
  while(theUART.LSR & theUART.LSR_RDR_DATA) {
    /* Dump data from RX FIFO */
    regVal = theUART.RBR;
  }
  /* Set the initialised flag in the protocol control block */
  pcb.initialised = 1;
  pcb.baudrate = baudrate;

  /* Enable the UART Interrupt */
  NVIC_EnableIRQ(UART_IRQn);
  theUART.IER = theUART.IER_RBR_Interrupt_Enabled | theUART.IER_RLS_Interrupt_Enabled;
} // uartInit

#endif // if 0
