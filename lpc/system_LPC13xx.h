/**************************************************************************//**
* @file     system_LPC13xx.h
* @brief    CMSIS Cortex-M3 Device Peripheral Access Layer Header File
*           for the NXP LPC13xx Device Series
* @version  V1.01
* @date     19. October 2009
*
* @note
* Copyright (C) 2009 ARM Limited. All rights reserved.
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


#ifndef __SYSTEM_LPC13xx_H
#define __SYSTEM_LPC13xx_H

#include <stdint.h>

/** will get called before constructors
 */
void SystemInit (void);

#endif /* __SYSTEM_LPC13x_H */
