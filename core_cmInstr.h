/**************************************************************************//**
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

#ifndef __CORE_CMINSTR_H__
#define __CORE_CMINSTR_H__

/* GNU gcc specific functions */

#define INTRINSICALLY __attribute__((always_inline)) static inline

#define SIMPLEOP(mne) INTRINSICALLY void __ ## mne(void){ __asm volatile (# mne); }

/** @see cm3/arm instruction manual for what these do.*/
SIMPLEOP(NOP)

SIMPLEOP(WFI)
SIMPLEOP(WFE)
SIMPLEOP(SEV)
// isb only useful if running from ram that might get modified.
SIMPLEOP(ISB)
SIMPLEOP(DSB)
SIMPLEOP(DMB)

#define SINGLEOP(mne)  INTRINSICALLY uint32_t __ ## mne(uint32_t value){                             \
    uint32_t result; __asm volatile (# mne " %0, %1" : "=r" (result) : "r" (value));  return result; \
}

/** @returns  byte reversed value of @param value */
SINGLEOP(REV)

/** @returns half-swapped @param value */
SINGLEOP(REV16)

SINGLEOP(REVSH)

// fancier cores have bit reverser:
SINGLEOP(RBIT)

/** \brief  LDR Exclusive (8 bit)
 */
INTRINSICALLY uint8_t __LDREXB(volatile uint8_t *addr){
  uint8_t result;
  __asm volatile ("ldrexb %0, [%1]" : "=r" (result) : "r" (addr));

  return result;
}


/** \brief  LDR Exclusive (16 bit)
 *
 *   This function performs a exclusive LDR command for 16 bit values.
 *
 *   \param [in]    ptr  Pointer to data
 *   \return        value of type uint16_t at (*ptr)
 */
INTRINSICALLY uint16_t __LDREXH(volatile uint16_t *addr){
  uint16_t result;

  __asm volatile ("ldrexh %0, [%1]" : "=r" (result) : "r" (addr));

  return result;
}


/** \brief  LDR Exclusive (32 bit)
 *
 *   This function performs a exclusive LDR command for 32 bit values.
 *
 *   \param [in]    ptr  Pointer to data
 *   \return        value of type uint32_t at (*ptr)
 */
INTRINSICALLY uint32_t __LDREXW(volatile uint32_t *addr){
  uint32_t result;

  __asm volatile ("ldrex %0, [%1]" : "=r" (result) : "r" (addr));

  return result;
}


/** \brief  STR Exclusive (8 bit)
 *
 *   This function performs a exclusive STR command for 8 bit values.
 *
 *   \param [in]  value  Value to store
 *   \param [in]    ptr  Pointer to location
 *   \return          0  Function succeeded
 *   \return          1  Function failed
 */
INTRINSICALLY uint32_t __STREXB(uint8_t value, volatile uint8_t *addr){
  uint32_t result;

  __asm volatile ("strexb %0, %2, [%1]" : "=&r" (result) : "r" (addr), "r" (value));

  return result;
}


/** \brief  STR Exclusive (16 bit)
 *
 *   This function performs a exclusive STR command for 16 bit values.
 *
 *   \param [in]  value  Value to store
 *   \param [in]    ptr  Pointer to location
 *   \return          0  Function succeeded
 *   \return          1  Function failed
 */
INTRINSICALLY uint32_t __STREXH(uint16_t value, volatile uint16_t *addr){
  uint32_t result;

  __asm volatile ("strexh %0, %2, [%1]" : "=&r" (result) : "r" (addr), "r" (value));

  return result;
}


/** \brief  STR Exclusive (32 bit)
 *
 *   This function performs a exclusive STR command for 32 bit values.
 *
 *   \param [in]  value  Value to store
 *   \param [in]    ptr  Pointer to location
 *   \return          0  Function succeeded
 *   \return          1  Function failed
 */
INTRINSICALLY uint32_t __STREXW(uint32_t value, volatile uint32_t *addr){
  uint32_t result;

  __asm volatile ("strex %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value));

  return result;
}

SIMPLEOP(CLREX);

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
     uint32_t __RES, __ARG1 = (ARG1);                                     \
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
     uint32_t __RES, __ARG1 = (ARG1);                                     \
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
INTRINSICALLY uint8_t __CLZ(uint32_t value){
  uint8_t result;

  __asm volatile ("clz %0, %1" : "=r" (result) : "r" (value));

  return result;
}

#endif // ifndef __CORE_CMINSTR_H__
