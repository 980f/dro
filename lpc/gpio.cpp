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
#include "syscon.h" // for clock enable.


using namespace LPC;


/*------------- General Purpose Input/Output (GPIO) --------------------------
12 bits each port, an architecturally fixed limit due to field access mechanism */
struct GPIO_PORT {
  union {
    /** address is anded with port bits, in or out */
    volatile uint32_t MASKED_ACCESS[4096];
    /** access all bits using final member of MASKED_ACCESS */
    struct {
      SKIPPED RESERVED0[4095];
      SFR DATA;
    };
  };
  SKIPPED RESERVED1[4096];//16k gap
  SFR DIR; //0x500p8000
  SFR IS;
  SFR IBE;
  SFR IEV;

  SFR IE;
  SFR RIS;
  SFR MIS;
  SFR IC;
};

/** to get rid of gobs of duplicated code, paired with switch statements: */
static inline GPIO_PORT *portPointer(PortNumber portNum){
  return reinterpret_cast<GPIO_PORT *>(0x50000000 + (portNum << 16));
}
// used static above as it doesn't guard against bad argument.

/** turn clock on to gpio and iocon blocks. */

void GPIO::Init (void){
  ClockController<6>(1); // gpio clock bit on.
  ClockController<16>(1); // iocon has to be turned on somewhere, might as well be here.
  // FYI: no jtag in these parts, just SWD.
}

//void GPIO::SetDir(PortNumber portNum, BitNumber bitPosi, bool outputter){
//  if(isLegalPort(portNum )) {
//    GPIO_PORT *lpcport = portPointer(portNum);
//    register uint32_t bitpicker = (1 << bitPosi);
//    // for some unpubished reason we go throught the effort of only sending changes. perhaps something happens on a write to the dir even if the bit's value doesn't change?
//    if(lpcport->DIR & bitpicker) {
//      if(! outputter) {
//        lpcport->DIR &= ~bitpicker;
//      }
//    } else { // presently 0
//      if(outputter) { // want 1
//        lpcport->DIR |= bitpicker;
//      }
//    }
//  }
//} // GPIO::SetDir


//void GPIO::SetValue(PortNumber portNum, BitNumber bitPosi, bool bitVal){
//  if(isLegalPort(portNum )) {
//    portPointer(portNum)->MASKED_ACCESS[(1 << bitPosi)] = bitVal ? ~0 : 0;
//  }
//}

//bool GPIO::GetValue(PortNumber portNum, BitNumber bitPosi){
//  if(isLegalPort(portNum)) {
//    return portPointer(portNum)->MASKED_ACCESS[(1 << bitPosi)] != 0;
//  } else {
//    return false;
//  }
//}


///*****************************************************************************
//** Function name:    GPIOSetInterrupt
//**
//** Descriptions:    Set interrupt sense, event, etc.
//**            edge or level, 0 is edge, 1 is level
//**            single or double edge, 0 is single, 1 is double
//**            active high or low, etc.
//**
//** parameters:      port num, bit position, sense, single/double, polarity
//** Returned value:    None
//**
//*****************************************************************************/
//void GPIO::SetInterrupt(PortNumber portNum, BitNumber bitPosi, uint32_t sense, uint32_t single, uint32_t event){
//  if(isLegalPort(portNum)) {
//    GPIO_PORT *lpcport = portPointer(portNum);

//    if(sense == 0) {
//      lpcport->IS &= ~(1 << bitPosi);
//      /* single or double only is relevent when sense is 0(edge trigger). */
//      if(single == 0) {
//        lpcport->IBE &= ~(1 << bitPosi);
//      } else {
//        lpcport->IBE |= (1 << bitPosi);
//      }
//    } else {
//      lpcport->IS |= (1 << bitPosi);
//    }
//    if(event == 0) {
//      lpcport->IEV &= ~(1 << bitPosi);
//    } else {
//      lpcport->IEV |= (1 << bitPosi);
//    }
//  }
//} // GPIO::SetInterrupt

///*****************************************************************************
//** Function name:    GPIOIntEnable
//**
//** Descriptions:    Enable Interrupt Mask for a port pin.
//**
//** parameters:      port num, bit position
//** Returned value:    None
//**
//*****************************************************************************/
//void GPIO::IntEnable(PortNumber portNum, BitNumber bitPosi){
//  if(isLegalPort(portNum)) {
//    portPointer(portNum)->IE |= (1 << bitPosi);
//  }
//}

///*****************************************************************************
//** Function name:    GPIOIntDisable
//**
//** Descriptions:    Disable Interrupt Mask for a port pin.
//**
//** parameters:      port num, bit position
//** Returned value:    None
//**
//*****************************************************************************/
//void GPIO::IntDisable(PortNumber portNum, BitNumber bitPosi){
//  if(isLegalPort(portNum)) {
//    portPointer(portNum)->IE &= ~(1 << bitPosi);
//  }
//}

///*****************************************************************************
//** Function name:    GPIOIntStatus
//**
//** Descriptions:    Get Interrupt status for a port pin.
//**
//** parameters:      port num, bit position
//** @returns whether the pin's interrupt is active
//**
//*****************************************************************************/

//bool GPIO::IntStatus(PortNumber portNum, BitNumber bitPosi){
//  if(isLegalPort(portNum)) {
//    if(portPointer(portNum)->MIS & (1 << bitPosi)) {
//      return 1;
//    }
//  }
//  return 0;
//}

///*****************************************************************************
//** Function name:    GPIOIntClear
//**
//** Descriptions:    Clear Interrupt for a port pin.
//**
//** parameters:      port num, bit position
//** Returned value:    None
//**
//*****************************************************************************/
//void GPIO::IntClear(PortNumber portNum, BitNumber bitPosi){
//  if(isLegalPort(portNum)) {
//    portPointer(portNum)->IC |= (1 << bitPosi);
//  }
//}

