#ifndef __CORE_CMINSTR_H__
#define __CORE_CMINSTR_H__
//heavily modified by andyh for easier visual comprehension, and for cross compiling on linux for sanity checking.
/**
* @file     core_cmInstr.h
* @brief    CMSIS Cortex-M Core Instruction Access Header File
* @version  V2.01
* @date     06. December 2010
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

// NXP Updated to add & to prefix result for byte and half word strex functions for GNU Compatibility

/* GNU gcc specific syntax */

#define INTRINSICALLY __attribute__((always_inline)) static inline

#if __linux__
#define SIMPLEOP(mne) void __ ## mne(void){}
#define SINGLEOP(mne)  uint32_t __ ## mne(uint32_t value){ return 0; }
#define MNE(mne)
#else
#define SIMPLEOP(mne) INTRINSICALLY void __ ## mne(void){ __asm volatile (# mne); }
#define SINGLEOP(mne)  INTRINSICALLY unsigned __ ## mne(unsigned value){                             \
    uint32_t result; __asm volatile (# mne " %0, %1" : "=r" (result) : "r" (value));  return result; \
}
// the above despite all the inline keywords was made into a callable function,so:
#define MNE(mne) __asm volatile (# mne)
#endif



/** @see cm3/arm instruction manual for what these do.*/
SIMPLEOP(NOP)

SIMPLEOP(WFI)
SIMPLEOP(WFE)
SIMPLEOP(SEV)
// isb only useful if running from ram that might get modified.
SIMPLEOP(ISB)
SIMPLEOP(DSB)
SIMPLEOP(DMB)

// leaving out all the load and store exclusive stuff, need to write the whole pairing in assembler to ensure always used nicely and relatively optimally.


#if 0 // expose these at need.
/** @returns  byte reversed value of @param value */
SINGLEOP(REV)

/** @returns half-swapped @param value */
SINGLEOP(REV16)

SINGLEOP(REVSH)

// fancier cores have bit reverser:
SINGLEOP(RBIT)


/** \brief  Signed Saturate
 *
 *   This function saturates a signed value.
 *
 *   \param [in]  value  Value to be saturated
 *   \param [in]    sat  Bit position to saturate to (1..32)
 *   \return             Saturated value
 */
#define __SSAT(ARG1, ARG2)                                                \
  ({                                                                      \
     unsigned __RES, __ARG1 = (ARG1);                                     \
     __asm("ssat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1)); \
     __RES;                                                               \
   }                                                                      \
  )


/** \brief  Unsigned Saturate
 *
 *   This function saturates an unsigned value.
 *
 *   \param [in]  value  Value to be saturated
 *   \param [in]    sat  Bit position to saturate to (0..31)
 *   \return             Saturated value
 */
#define __USAT(ARG1, ARG2)                                                \
  ({                                                                      \
     unsigned __RES, __ARG1 = (ARG1);                                     \
     __asm("usat %0, %1, %2" : "=r" (__RES) :  "I" (ARG2), "r" (__ARG1)); \
     __RES;                                                               \
   }                                                                      \
  )


/** \brief  Count leading zeros
 *
 *   This function counts the number of leading zeros of a data value.
 *
 *   \param [in]  value  Value to count the leading zeros
 *   \return             number of leading zeros in value
 */
INTRINSICALLY unsigned __CLZ(unsigned value){
  unsigned result;

  __asm volatile ("clz %0, %1" : "=r" (result) : "r" (value));

  return result;
}
#endif // if 0

#endif // ifndef __CORE_CMINSTR_H__
