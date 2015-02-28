#include "core_itm.h"

// very unclear as to who is supposed to write to this varble:
volatile int32_t ITM_RxBuffer;                    /*!< external variable to receive characters                    */
#define ITM_RXBUFFER_EMPTY    0x5AA55AA5 /*!< value identifying ITM_RxBuffer is ready for next character */

/** \brief  ITM Send Character
 *
 *   This function transmits a character via the ITM channel 0.
 *   It just returns when no debugger is connected that has booked the output.
 *   It is blocking when a debugger is connected, but the previous character send is not transmitted.
 *
 *   \param [in]     ch  Character to transmit
 *   \return             Character to transmit
 */
uint32_t ITM_SendChar (uint32_t ch){
//  if((CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk) && /* Trace enabled */
//     (ITM->TCR & ITM_TCR_ITMENA_Msk) && /* ITM enabled */
//     (ITM->TER & (1UL << 0))) {  /* ITM Port #0 enabled */
//    while(ITM->PORT[0].u32 == 0) {
//    }
//    ITM->PORT[0].u8 = (uint8_t) ch;
//  }
  return ch;
}

/**@returns whether a character is available */
bool ITM_CheckChar (void){
  return ITM_RxBuffer != ITM_RXBUFFER_EMPTY;
}

/** \brief  ITM Receive Character
 *
 *   This function inputs a character via external variable ITM_RxBuffer.
 *   It just returns when no debugger is connected that has booked the output.
 *   It is blocking when a debugger is connected, but the previous character send is not transmitted.
 *
 *   \return             Received character
 *   \return         -1  No character received
 */
int32_t ITM_ReceiveChar (void){
  if(ITM_CheckChar()) {
    int32_t ch = ITM_RxBuffer;
    ITM_RxBuffer = ITM_RXBUFFER_EMPTY;       /* ready for next character */
    return ch;
  } else {
    return -1;
  }
}
