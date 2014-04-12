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
#include <stdint.h>
#include "peripheraltypes.h"

namespace LPC {
/*------------- System Control (SYSCON) --------------------------------------*/
  struct SYSCON {
    volatile uint32_t SYSMEMREMAP; /* Sys mem. Remap, Offset 0x0   */
    SFR PRESETCTRL;
    SFR SYSPLLCTRL; /* Sys PLL control              */
    SFR SYSPLLSTAT;
    SFR USBPLLCTRL; /* USB PLL control, offset 0x10 */
    SFR USBPLLSTAT;
    SKIPPED RESERVED0[2];

    SFR SYSOSCCTRL; /* Offset 0x20 */
    SFR WDTOSCCTRL;
    SFR IRCCTRL;
    SKIPPED RESERVED1[1];
    SFR SYSRESSTAT; /* Offset 0x30 */
    SKIPPED RESERVED2[3];
    SFR SYSPLLCLKSEL; /* Offset 0x40 */
    SFR SYSPLLCLKUEN;
    SFR USBPLLCLKSEL;
    SFR USBPLLCLKUEN;
    SKIPPED RESERVED3[8];

    SFR MAINCLKSEL; /* Offset 0x70 */
    SFR MAINCLKUEN;
    SFR SYSAHBCLKDIV;
    SKIPPED RESERVED4[1];

    SFR SYSAHBCLKCTRL; /* Offset 0x80 */
    SKIPPED RESERVED5[4];

    SFR SSP0CLKDIV;
    SFR UARTCLKDIV;
    SFR SSP1CLKDIV; /*!< Offset: 0x09C SSP1 clock divider (R/W) */
    SKIPPED RESERVED6[3];
    SFR TRACECLKDIV;

    SFR SYSTICKCLKDIV; /* Offset 0xB0 */
    SKIPPED RESERVED7[3];

    SFR USBCLKSEL; /* Offset 0xC0 */
    SFR USBCLKUEN;
    SFR USBCLKDIV;
    SKIPPED RESERVED8[1];
    SFR WDTCLKSEL; /* Offset 0xD0 */
    SFR WDTCLKUEN;
    SFR WDTCLKDIV;
    SKIPPED RESERVED9[1];
    SFR CLKOUTCLKSEL; /* Offset 0xE0 */
    SFR CLKOUTUEN;
    SFR CLKOUTDIV;
    SKIPPED RESERVED10[5];

    SFR PIOPORCAP0; /* Offset 0x100 */
    SFR PIOPORCAP1;
    SKIPPED RESERVED11[18];

    SFR BODCTRL; /* Offset 0x150 */
    SKIPPED RESERVED12[1];
    SFR SYSTCKCAL;
    SKIPPED RESERVED13[41];

    SFR STARTAPRP0; /* Offset 0x200 */
    SFR STARTERP0;
    SFR STARTRSRP0CLR;
    SFR STARTSRP0;
    SFR STARTAPRP1;
    SFR STARTERP1;
    SFR STARTRSRP1CLR;
    SFR STARTSRP1;
    SKIPPED RESERVED14[4];

    SFR PDSLEEPCFG; /* Offset 0x230 */
    SFR PDAWAKECFG;
    SFR PDRUNCFG;
    SKIPPED RESERVED15[110];
    /** const in the deepest sense, no need for read-only */
    SKIPPED DEVICE_ID;
  };

/*------------- Pin Connect Block (IOCON) --------------------------------*/
  struct IOCON {
    SFR PIO2_6;
    SKIPPED RESERVED0[1];
    SFR PIO2_0;
    SFR RESET_PIO0_0;
    SFR PIO0_1;
    SFR PIO1_8;
    SKIPPED RESERVED1[1];
    SFR PIO0_2;

    SFR PIO2_7;
    SFR PIO2_8;
    SFR PIO2_1;

    SFR PIO0_3;
    SFR PIO0_4;
    SFR PIO0_5;

    SFR PIO1_9;

    SFR PIO3_4;

    SFR PIO2_4;
    SFR PIO2_5;

    SFR PIO3_5;

    SFR PIO0_6;
    SFR PIO0_7;

    SFR PIO2_9;
    SFR PIO2_10;
    SFR PIO2_2;

    SFR PIO0_8;
    SFR PIO0_9;
    SFR PIO0_10;
    SFR PIO1_10;
    SFR PIO2_11;
    SFR PIO0_11;
    SFR PIO1_0;
    SFR PIO1_1;

    SFR PIO1_2;
    SFR PIO3_0;
    SFR PIO3_1;
    SFR PIO2_3;
    SFR PIO1_3;

    SFR PIO1_4;
    SFR PIO1_11;
    SFR PIO3_2;

    SFR PIO1_5;
    SFR PIO1_6;
    SFR PIO1_7;
    SFR PIO3_3;
    SFR SCK_LOC; /*!< Offset: 0x0B0 SCK pin location select Register (R/W) */
    SFR DSR_LOC; /*!< Offset: 0x0B4 DSR pin location select Register (R/W) */
    SFR DCD_LOC; /*!< Offset: 0x0B8 DCD pin location select Register (R/W) */
    SFR RI_LOC; /*!< Offset: 0x0BC RI pin location Register (R/W) */
  };

/*------------- Power Management Unit (PMU) --------------------------*/
  struct PMU {
    SFR PCON;
    SFR GPREG0;
    SFR GPREG1;
    SFR GPREG2;
    SFR GPREG3;
    SFR GPREG4;
  };

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
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
    SKIPPED RESERVED1[4096];
    SFR DIR;
    SFR IS;
    SFR IBE;
    SFR IEV;
    SFR IE;
    SFR RIS;
    SFR MIS;
    SFR IC;
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
      const SFR  RBR;
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
      const SFR  IIR;
      SFR FCR;
    };
    SFR LCR;
    SFR MCR;
    const SFR  LSR;
    const SFR  MSR;
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
    const SFR  FIFOLVL;
  };

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
  struct SSP {
    SFR CR0;
    SFR CR1;
    SFR DR;
    const SFR  SR;
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

/*------------- Watchdog Timer (WDT) -----------------------------------------*/
  struct WDT {
    SFR MOD;
    SFR TC;
    SFR FEED;
    const SFR TV;
    SKIPPED RESERVED0;
    SFR WARNINT; /*!< Offset: 0x014 Watchdog timer warning int. register (R/W) */
    SFR WINDOW; /*!< Offset: 0x018 Watchdog timer window value register (R/W) */
  };

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
  struct ADC{
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
  } ;

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

/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define LPC_FLASH_BASE        (0x00000000UL)
#define LPC_RAM_BASE          (0x10000000UL)
#define LPC_APB0_BASE         (0x40000000UL)
#define LPC_AHB_BASE          (0x50000000UL)

// 16k blocks of address space are allocated per apb device
#define apb0Device(offset) (LPC_APB0_BASE + (offset << 14))


DeclareSingle(I2C, apb0Device(0));
DeclareSingle(WDT, apb0Device(1));
DeclareSingle(UART, apb0Device(2));
DeclareUnit(TMR, 16B0, apb0Device(3));
DeclareUnit(TMR, 16B1, apb0Device(4));
DeclareUnit(TMR, 32B0, apb0Device(5));
DeclareUnit(TMR, 32B1, apb0Device(6));
DeclareSingle(ADC, apb0Device(7));
DeclareSingle(PMU, apb0Device(14));
DeclareUnit(SSP, 0, apb0Device(16));
DeclareUnit(SSP, 1, apb0Device(22));
DeclareSingle(IOCON, apb0Device(17));
DeclareSingle(SYSCON, apb0Device(18));
DeclareSingle(USB, apb0Device(8));

#endif // ifndef lpc13xx_H
