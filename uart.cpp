#include "uart.h"
#include "lpcperipheral.h" //the peripheral is wholly hidden within this module.
using namespace LPC;

/** 0=disabled else divide by 1:255.
 Could use a simple SFR, but would still want a function to validate the range. */
SFRfield <sysConReg(0x98),0,8>uartClockDivider;


#include "nvic.h"

constexpr unsigned uartIrq=3;


namespace LPC {
/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
struct UART {
  /** actually overlapped registers. For some stupid reason they are maintaining compatibility with code that would have been written for other processors. SO that is cute but they could have also provided direct access so that the aggravating overlay of registers isn't necessary.*/
  union {
    const SFR RBR;
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
}
DefineSingle(UART, apb0Device(2));

Uart::Uart(){
  //todo: turn on clock, do any basic housekeeping.
}

unsigned Uart::setBaud(unsigned hertz) const {
  return 0;
}

void Uart::setFraming(const char *coded) const{


}

bool Uart::receive(int /*incoming*/){
  return false; //disable reception if improper access is made to base class.
}

int Uart::send(){
  return -1;//disable transmitter if improper access is made to base class.
}

#if 0
void UART_IRQHandler(void)
{
  uint8_t IIRValue, LSRValue;
  uint8_t Dummy = Dummy;

  IIRValue = UART_U0IIR;
  IIRValue &= ~(UART_U0IIR_IntStatus_MASK); /* skip pending bit in IIR */
  IIRValue &= UART_U0IIR_IntId_MASK;        /* check bit 1~3, interrupt identification */

  // 1.) Check receiver line status
  if (IIRValue == UART_U0IIR_IntId_RLS)
  {
    LSRValue = UART_U0LSR;
    // Check for errors
    if (LSRValue & (UART_U0LSR_OE | UART_U0LSR_PE | UART_U0LSR_FE | UART_U0LSR_RXFE | UART_U0LSR_BI))
    {
      /* There are errors or break interrupt */
      /* Read LSR will clear the interrupt */
      pcb.status = LSRValue;
      Dummy = UART_U0RBR;	/* Dummy read on RX to clear interrupt, then bail out */
      return;
    }
    // No error and receive data is ready
    if (LSRValue & UART_U0LSR_RDR_DATA)
    {
      /* If no error on RLS, normal ready, save into the data buffer. */
      /* Note: read RBR will clear the interrupt */
      uartRxBufferWrite(UART_U0RBR);
    }
  }

  // 2.) Check receive data available
  else if (IIRValue == UART_U0IIR_IntId_RDA)
  {
    // Add incoming text to UART buffer
    uartRxBufferWrite(UART_U0RBR);
  }

  // 3.) Check character timeout indicator
  else if (IIRValue == UART_U0IIR_IntId_CTI)
  {
    /* Bit 9 as the CTI error */
    pcb.status |= 0x100;
  }

  // 4.) Check THRE (transmit holding register empty)
  else if (IIRValue == UART_U0IIR_IntId_THRE)
  {
    /* Check status in the LSR to see if valid data in U0THR or not */
    LSRValue = UART_U0LSR;
    if (LSRValue & UART_U0LSR_THRE)
    {
      pcb.pending_tx_data = 0;
    }
    else
    {
      pcb.pending_tx_data= 1;
    }
  }
  return;
}

/**************************************************************************/
void uartSendByte (uint8_t byte)
{
  /* THRE status, contain valid data */
  while ( !(UART_U0LSR & UART_U0LSR_THRE) );
  UART_U0THR = byte;

  return;
}

void uartInit(uint32_t baudrate)
{
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
  UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                UART_U0LCR_Stop_Bit_Select_1Bits |
                UART_U0LCR_Parity_Disabled |
                UART_U0LCR_Parity_Select_OddParity |
                UART_U0LCR_Break_Control_Disabled |
                UART_U0LCR_Divisor_Latch_Access_Enabled);

  /* Baud rate */
  regVal = SCB_UARTCLKDIV;
  fDiv = (((CFG_CPU_CCLK * SCB_SYSAHBCLKDIV)/regVal)/16)/baudrate;

  UART_U0DLM = fDiv / 256;
  UART_U0DLL = fDiv % 256;

  /* Set DLAB back to 0 */
  UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                UART_U0LCR_Stop_Bit_Select_1Bits |
                UART_U0LCR_Parity_Disabled |
                UART_U0LCR_Parity_Select_OddParity |
                UART_U0LCR_Break_Control_Disabled |
                UART_U0LCR_Divisor_Latch_Access_Disabled);

  /* Enable and reset TX and RX FIFO. */
  UART_U0FCR = (UART_U0FCR_FIFO_Enabled |
                UART_U0FCR_Rx_FIFO_Reset |
                UART_U0FCR_Tx_FIFO_Reset);

  /* Read to clear the line status. */
  regVal = UART_U0LSR;

  /* Ensure a clean start, no data in either TX or RX FIFO. */
  while (( UART_U0LSR & (UART_U0LSR_THRE|UART_U0LSR_TEMT)) != (UART_U0LSR_THRE|UART_U0LSR_TEMT) );
  while ( UART_U0LSR & UART_U0LSR_RDR_DATA )
  {
    /* Dump data from RX FIFO */
    regVal = UART_U0RBR;
  }

  /* Set the initialised flag in the protocol control block */
  pcb.initialised = 1;
  pcb.baudrate = baudrate;

  /* Enable the UART Interrupt */
  NVIC_EnableIRQ(UART_IRQn);
  UART_U0IER = UART_U0IER_RBR_Interrupt_Enabled | UART_U0IER_RLS_Interrupt_Enabled;

  return;
}

#endif
