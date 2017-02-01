#include "uart.h"
#include "peripheraltypes.h"
#include "nvic.h"

#include "clocks.h"

using namespace SAM;

const unsigned UartPid=8;


/** a collection of a few bits: */
const unsigned ControlAddress=0x400E0600;

static constexpr unsigned uartRegister(unsigned offset){
  return ControlAddress+offset;
}

/** bit that do things when a 1 is written to them: */
enum ControlBit {
RSTRX=2, // Reset Receiver 1 = The receiver logic is reset and disabled. If a character is being received, the reception is aborted.
RSTTX=3, // Reset Transmitter 1 = The transmitter logic is reset and disabled. If a character is being transmitted, the transmission is aborted.
RXEN=4,  // Receiver Enable 1 = The receiver is enabled (if RXDIS is 0.)
RXDIS=5, // Receiver Disable 1 = The receiver is disabled. If a character is being processed and RSTRX is not set, the character is completed before the receiver is stopped.
TXEN=6,  // Transmitter Enable 1 = The transmitter is enabled (if TXDIS is 0.)
TXDIS=7, // Transmitter Disable 1 = The transmitter is disabled. If a character is being processed and a character has been written in the UART_THR (and RSTTX is not set), both characters are completed before the transmitter is stopped.
RSTSTA=8, // Reset Status Bits 1 = Resets the status bits PARE, FRAME and OVRE in the UART_SR.
};

static void actuate(ControlBit cbit){
  *atAddress(ControlAddress)= bitMask(cbit);
}


/** this device has an interesting feature, setting the 'enable' bit generates an interrupt, allowing your startup code to be in your isr.
amongst other niceties that gives you a perhaps 'free' atomic access to your transmit buffer. */
enum Interrupt {
//there is a Feature at 8
RXRDY=0 , // character available
TXRDY=1 , // transmit buffer empty
ENDRX=3 , // DMA End of Receive Transfer
ENDTX=4 , // DMA End of Transmit
OVRE=5 , //  Overrun Error, comes instead of a second RXRDY (sicne RXRDY is still high)
FRAME=6 , //  Framing Error, synched with RSRDY
PARE=7 , //  Parity Error, synched with RXRDY
TXEMPTY=9 , // transmitter is idle, nothing shifting and nothing in the buffer register.
TXBUFE=11 , // DMA TX Buffer Empty
RXBUFF=12 , // DMA RX Buffer Full
};

struct UartStatus {
  unsigned RXRDY:1;
  unsigned TXRDY:1;
  unsigned :1;
  unsigned ENDRX:1;
  unsigned ENDTX:1;

  unsigned OVRE:1;
  unsigned FRAME:1;
  unsigned PARE:1;
  unsigned :1;
  unsigned TXEMPTY:1;
  unsigned TXBUFE:1;
  unsigned RXBUFF:1;
  unsigned :20;
};

void Uart::isr() const{
  union {
    unsigned word;
    UartStatus bit;
  } status;

  status.word=*atAddress(uartRegister(0x14));//read just once per interrupt

  if(status.bit.TXRDY){
    if(int nextch = (*send)() >= 0) {//#trusting that transmit will not be enabled if send is null
      *atAddress(uartRegister(0)) = unsigned(nextch);
    }
  }

  BitField<OVRE,PARE> errs(status.word);
  if(errs) {//receive error
    if(receive){//COA
      (*receive)(~int(errs)); // inform user code, althought the value will not be any clear thing.
    }
    actuate(RSTSTA);
  } else if(status.bit.RXRDY){
    if(receive){
      bool ok=(*receive)(*atAddress(uartRegister(0x18)));
      if(!ok){
        //need to disable reception else we lose characters.
        //the person reading the chars will have to reenable reception.
        reception(false);
      }
    }
  }

}

Uart::Uart(Uart::Receiver receiver, Uart::Sender sender):
  receive(receiver),
  send(sender)
{
  //#nada- want a const init to do nothing.
}

static unsigned baudy(unsigned hertz, unsigned sysFreq){
  return sysFreq/hertz*16;
}

unsigned Uart::setBaud(unsigned hertz, unsigned sysFreq) const {
  if(sysFreq==0){
    sysFreq=clockRate(0);//master clock is the source
  }
  unsigned divider=baudy(hertz,sysFreq);
  //todo: range check, max 65535, min 1, else clock won't tick.
  *atAddress(uartRegister(0x20))=divider;
  return baudy(divider,sysFreq);
}

bool Uart::setFraming(const char *coded) const {
  unsigned numbits = *coded++ - '0';
  if(numbits!=8){
    return false;
  }

  unsigned parity=0;
  switch(*coded++) {
  default:
  case 0:
    //defective config string
    return false;

  case 'N':
    parity = 4;
    break;
  case 'O':
    parity = 1;
    break;
  case 'E':
    parity = 0;
    break;
  case 'M':
    parity = 3;
    break;
  case 'S':
    parity = 2;
    break;
  } // switch
  *atAddress(uartRegister(4))= parity<<9;

  int stopbits = *coded++ - '0';
  if(stopbits != 1) {
    return false;
  }
  return true;
}

void Uart::beTransmitting(bool enabled) const {
  if(enabled){
    actuate(RSTSTA);
    *atAddress(uartRegister(8))=BitWad<ENDRX,ENDTX,OVRE,FRAME,PARE,TXEMPTY,TXBUFE,RXBUFF>::mask;//unbuffered
    *atAddress(uartRegister(8))=BitWad<ENDRX,ENDTX,OVRE,FRAME,PARE,TXEMPTY,TXBUFE,RXBUFF>::mask;//with dma
  }

  //the 'first char' handling is in the isr, as the following actuation triggers an interrupt.
  actuate(enabled?TXEN:TXDIS);
}

void Uart::reception(bool enabled) const {
  if(enabled){
    //clear errors and enable interrupts
    actuate(RSTSTA);
    *atAddress(uartRegister(8))=BitWad<RXRDY,TXRDY,OVRE,/*FRAME and PARE are contingent upon RXRDY*/ TXEMPTY>::mask;//no DMA
  }
  actuate(enabled?RXEN:RXDIS);
}

void Uart::irq(bool enabled) const {
//nvic interrupt:
  Irq<UartPid>irq;
  irq=enabled;
}

