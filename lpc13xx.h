/****************************************************************************
*   LPC13xx.h - 2011-04-28
*
*   Based on original by NXP with modifications by Code Red Technologies
*
*   Project: NXP LPC13xx software example
*
*   Description:
*     CMSIS Cortex-M3 Core Peripheral Access Layer Header File for
*     NXP LPC13xx Device Series
*
****************************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
****************************************************************************/

#ifndef lpc13xx_H
#define lpc13xx_H

#include <stdint.h>
#include "peripheraltypes.h"


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          3         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/


namespace LPC {
/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
//  constexpr unsigned FLASH_BASE(0x00000000UL);
//  constexpr unsigned RAM_BASE(0x10000000UL);

// 16k blocks of address space are allocated per apb device
  constexpr unsigned apb0Device(unsigned unitNumber){
    return 0x40000000UL + (unitNumber << 14);
  }


/*------------- Power Management Unit (PMU) --------------------------*/
  struct PMU {
    SFR PCON;
    SFR GPREG0;
    SFR GPREG1;
    SFR GPREG2;
    SFR GPREG3;
    SFR GPREG4;
  };


/*------------- Timer (TMR) --------------------------------------------------*/
  struct TMR {
    SFR IR;
    SFR TCR;
    SFR TC;
    SFR PR;
    SFR PC;
    SFR MCR;
    SFR MR0;
    SFR MR1;
    SFR MR2;
    SFR MR3;
    SFR CCR;
    const SFR CR0;
    SKIPPED RESERVED1[3];
    SFR EMR;
    SKIPPED RESERVED2[12];
    SFR CTCR;
    SFR PWMC;
  };

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
  struct UART {
    /** actually overlapped registers.*/
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

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
  struct SSP {
    SFR CR0;
    SFR CR1;
    SFR DR;
    const SFR SR;
    SFR CPSR;
    SFR IMSC;
    SFR RIS;
    SFR MIS;
    SFR ICR;
  };

/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
  struct I2C {
    SFR CONSET;
    const SFR STAT;
    SFR DAT;
    SFR ADR0;
    SFR SCLH;
    SFR SCLL;
    SFR CONCLR;
    SFR MMCTRL;
    SFR ADR1;
    SFR ADR2;
    SFR ADR3;
    const SFR DATA_BUFFER;
    SFR MASK0;
    SFR MASK1;
    SFR MASK2;
    SFR MASK3;
  };



/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
  struct ADC {
    SFR CR;
    SFR GDR;
    SKIPPED RESERVED0;
    SFR INTEN;

    union {
      SFR DR[8];

      struct {
        const SFR DR0;
        const SFR DR1;
        const SFR DR2;
        const SFR DR3;
        const SFR DR4;
        const SFR DR5;
        const SFR DR6;
        const SFR DR7;
      };
    };
    const SFR STAT;
  };

/*------------- Universal Serial Bus (USB) -----------------------------------*/
  struct USB {
    const SFR DevIntSt; /* USB Device Interrupt Registers     */
    SFR DevIntEn;
    SFR DevIntClr;
    SFR DevIntSet;

    SFR CmdCode; /* USB Device SIE Command Registers   */
    const SFR CmdData;

    const SFR RxData; /* USB Device Transfer Registers      */
    SFR TxData;
    const SFR RxPLen;
    SFR TxPLen;
    SFR Ctrl;
    SFR DevFIQSel;
  };
} // end namespace


// DeclareSingle(I2C, apb0Device(0));
// DeclareSingle(WDT, apb0Device(1));
// DeclareSingle(UART, apb0Device(2));
// DeclareUnit(TMR, 16B0, apb0Device(3));
// DeclareUnit(TMR, 16B1, apb0Device(4));
// DeclareUnit(TMR, 32B0, apb0Device(5));
// DeclareUnit(TMR, 32B1, apb0Device(6));
// DeclareSingle(ADC, apb0Device(7));
// DeclareSingle(PMU, apb0Device(14));
// DeclareUnit(SSP, 0, apb0Device(16));
// DeclareUnit(SSP, 1, apb0Device(22));
// DeclareSingle(IOCON, apb0Device(17));
// DeclareSingle(SYSCON, apb0Device(18));
// DeclareSingle(USB, apb0Device(8));

/** enable an apb clock. each module will know its bit, or we will have to add an enum for the first argument here.*/
void setClock(int which, bool running = true);


#endif // ifndef lpc13xx_H
