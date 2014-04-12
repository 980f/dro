/*****************************************************************************
 *   gpio.c:  GPIO C file for NXP LPC13xx Family Microprocessors
 *
 *   Copyright(C) 2008, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.12.09  ver 1.01    GPIOSetValue() updated
 *   2008.07.20  ver 1.00    Preliminary version, first Release
 *
 *****************************************************************************/
#include "lpc13xx.h"      /* LPC13xx Peripheral Registers */
#include "gpio.h"
using namespace LPC;
/** to get rid of gobs of duplicated code, paired with switch statements: */
static inline GPIO_PORT *portPointer(PortNumber portNum){
  return reinterpret_cast<GPIO_PORT*> (0x50000000 + (portNum << 16));
}
//used static above as it doesn't guard against bad argument.

/*****************************************************************************
 * Function name: GPIOInit
 *
 * Descriptions: Initialize GPIO, install the
 * GPIO interrupt handler
 **
 ** parameters: None
 ** Returned value: true or false, return false if the VIC table
 ** is full and GPIO interrupt handler can be
 ** installed.
 **
 *****************************************************************************/
void GPIO::Init(void){
  /* Enable AHB clock to the GPIO domain. */
  theSYSCON->SYSAHBCLKCTRL |= (1 << 6);

#ifdef __JTAG_DISABLED
  LPC_IOCON->JTAG_TDO_PIO1_1 &= ~0x07;
  LPC_IOCON->JTAG_TDO_PIO1_1 |= 0x01;
#endif

}


/*****************************************************************************
 ** Function name:		GPIOSetDir
 **
 ** Descriptions:		Set the direction in GPIO port
 **
 ** parameters:			port num, bit position, direction (1 out, 0 input)
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIO::SetDir(PortNumber portNum, BitNumber bitPosi, bool outputter){
  /* if DIR is OUT(1), but GPIOx_DIR is not set, set DIR
    * to OUT(1); if DIR is IN(0), but GPIOx_DIR is set, clr
    * DIR to IN(0). All the other cases are ignored.
    * On port3(bit 0 through 3 only), no error protection if
    * bit value is out of range. */
   if(isLegalPort(portNum )){
    register uint32_t bitpicker = (1 << bitPosi);
     GPIO_PORT*lpcport = portPointer(portNum);

    if(lpcport->DIR & bitpicker) {
      if(!outputter) {
        lpcport->DIR &= ~bitpicker;
      }
    } else { //presently 0
      if(outputter) { //want 1
        lpcport->DIR |= bitpicker;
      }
    }
   }
} // GPIO::SetDir

/*****************************************************************************
 ** Function name:		GPIOSetValue
 **
 ** Descriptions:		Set/clear a bitvalue in a specific bit position
 **						in GPIO portX(X is the port number.)
 **
 ** parameters:			port num, bit position, bit value
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIO::SetValue(PortNumber portNum, BitNumber bitPosi, bool bitVal){
   if(isLegalPort(portNum )) {
    portPointer(portNum)->MASKED_ACCESS[(1 << bitPosi)] = bitVal ? ~0 : 0;
  }
}

bool GPIO::GetValue(PortNumber portNum, BitNumber bitPosi){
  if(isLegalPort(portNum)) {
    return portPointer(portNum)->MASKED_ACCESS[(1 << bitPosi)] != 0;
  } else {
    return false;
  }
}


/*****************************************************************************
 ** Function name:		GPIOSetInterrupt
 **
 ** Descriptions:		Set interrupt sense, event, etc.
 **						edge or level, 0 is edge, 1 is level
 **						single or double edge, 0 is single, 1 is double
 **						active high or low, etc.
 **
 ** parameters:			port num, bit position, sense, single/double, polarity
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIO::SetInterrupt(PortNumber portNum, BitNumber bitPosi, uint32_t sense, uint32_t single, uint32_t event){
  if(isLegalPort(portNum)) {
    GPIO_PORT*lpcport = portPointer(portNum);

    if(sense == 0) {
      lpcport->IS &= ~(1 << bitPosi);
      /* single or double only is relevent when sense is 0(edge trigger). */
      if(single == 0) {
        lpcport->IBE &= ~(1 << bitPosi);
      } else {
        lpcport->IBE |= (1 << bitPosi);
      }
    } else {
      lpcport->IS |= (1 << bitPosi);
    }
    if(event == 0) {
      lpcport->IEV &= ~(1 << bitPosi);
    } else {
      lpcport->IEV |= (1 << bitPosi);
    }
  }
} // GPIO::SetInterrupt

/*****************************************************************************
 ** Function name:		GPIOIntEnable
 **
 ** Descriptions:		Enable Interrupt Mask for a port pin.
 **
 ** parameters:			port num, bit position
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIO::IntEnable(uint32_t portNum, uint32_t bitPosi){
  if(isLegalPort(portNum)) {
    portPointer(portNum)->IE |= (1 << bitPosi);
  }
}

/*****************************************************************************
 ** Function name:		GPIOIntDisable
 **
 ** Descriptions:		Disable Interrupt Mask for a port pin.
 **
 ** parameters:			port num, bit position
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIO::IntDisable(uint32_t portNum, uint32_t bitPosi){
  if(isLegalPort(portNum)) {
    portPointer(portNum)->IE &= ~(1 << bitPosi);
  }
}

/*****************************************************************************
 ** Function name:		GPIOIntStatus
 **
 ** Descriptions:		Get Interrupt status for a port pin.
 **
 ** parameters:			port num, bit position
 ** Returned value:		None
 **
 *****************************************************************************/

bool GPIO::IntStatus(PortNumber portNum, BitNumber bitPosi){
  if(isLegalPort(portNum)) {
    if(portPointer(portNum)->MIS & (1 << bitPosi)) {
      return 1;
    }
  }
  return 0;
}

/*****************************************************************************
 ** Function name:		GPIOIntClear
 **
 ** Descriptions:		Clear Interrupt for a port pin.
 **
 ** parameters:			port num, bit position
 ** Returned value:		None
 **
 *****************************************************************************/
void GPIO::IntClear(uint32_t portNum, uint32_t bitPosi){
  if(isLegalPort(portNum)) {
    portPointer(portNum)->IC |= (1 << bitPosi);
  }
}

/******************************************************************************
 **                            End Of File
 ******************************************************************************/
