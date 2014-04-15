/**************************************************************************//**
* @file     core_cm3.c
* @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Source File
* @version  V2.00
* @date     13. September 2010
*
* @note
* Copyright (C) 2009-2010 ARM Limited. All rights reserved.
*
* @par
* ARM Limited (ARM) is supplying this software for use with Cortex-M
* processor based microcontrollers.  This file can be freely distributed
* within development tools that are supporting such ARM based processors.
*
* @par
* THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
* ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
*
******************************************************************************/

#include "core_cm3.h"
//#define InitCore(regname, addr) CM3:: regname & the ## regname(*reinterpret_cast<CM3:: regname *>(addr))


// InitCore(CoreDebug, 0xE000EDF0UL);  /*!< Core Debug configuration struct   */
// InitCore(InterruptType, SCS_BASE);        /*!< Interrupt Type Register           */
// InitCore(SCB, (SCS_BASE + 0x0D00UL));       /*!< SCB configuration struct          */
// #define SysTick             ((SysTick_Type *) SysTick_BASE)    /*!< SysTick configuration struct      */
// #define NVIC                ((NVIC_Type *) NVIC_BASE)       /*!< NVIC configuration struct         */
// InitCore(ITM, ITM_BASE);


