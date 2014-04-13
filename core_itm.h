#ifndef core_itm_h
#define core_itm_h 1
/** \ingroup  CMSIS_core_register
 *   \defgroup CMSIS_ITM CMSIS ITM
 *  Type definitions for the Cortex-M Instrumentation Trace Macrocell (ITM)
 *  @{
 */

#include "peripheraltypes.h"
/** Structured access to the Instrumentation Trace Macrocell Register (ITM).
 */
namespace CM3 {
struct ITM {
  union {
    SFR8 as8;                 /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 8-bit                   */
    SFR16 as16;                /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 16-bit                  */
    SFR as32;                /*!< Offset: 0x000 ( /W)  ITM Stimulus Port 32-bit                  */
  }
  PORT[32];                         /*!< Offset: 0x000 ( /W)  ITM Stimulus Port Registers               */
  uint32_t RESERVED0[864];
  SFR TER;                    /*!< Offset:       (R/W)  ITM Trace Enable Register                 */
  uint32_t RESERVED1[15];
  SFR TPR;                    /*!< Offset:       (R/W)  ITM Trace Privilege Register              */
  uint32_t RESERVED2[15];
  SFR TCR;                    /*!< Offset:       (R/W)  ITM Trace Control Register                */
  uint32_t RESERVED3[29];
  SFR IWR;                    /*!< Offset:       (R/W)  ITM Integration Write Register            */
  SFR IRR;                    /*!< Offset:       (R/W)  ITM Integration Read Register             */
  SFR IMCR;                   /*!< Offset:       (R/W)  ITM Integration Mode Control Register     */
  uint32_t RESERVED4[43];
  SFR LAR;                    /*!< Offset:       (R/W)  ITM Lock Access Register                  */
  SFR LSR;                    /*!< Offset:       (R/W)  ITM Lock Status Register                  */
  uint32_t RESERVED5[6];
  const SFR PID[8];                  /*!< Offset:       (R/ )  ITM Peripheral Identification Register #4 */
  // to get traditional offsets xor 4 into the index into PID[]
  const SFR CID[4];                  /*!< Offset:       (R/ )  ITM Component  Identification Register #0 */

/** utilities */
  /** send a char out the itm port, @returns the char sent.
   *   It does nothing when no debugger is connected that has hooked the output.
   *   It blocks when a debugger is connected, but the previous character sent is not transmitted.
*/
  static uint32_t sendChar (uint32_t ch);
   /** @returns whether a character is available */
  static bool available(void);
  /**  @returns a received character or -1 if no character received */
  int32_t ReceiveChar (void);
};

/* ITM Trace Privilege Register Definitions */

typedef SFRfield<SFRptr(0xE0000000UL, ITM, TPR), 0, 4> ITM_TPR_PRIVMASK;
// usage:  ITM_TPR_PRIVMASK mask; mask=value;

#define ITM_FIELD(regname, offset, width) typedef SFRfield < SFRptr (0xE0000000UL, ITM, regname), offset, width >
#define ITM_BIT(regname, offset) typedef SFRbit < SFRptr (0xE0000000UL, ITM, regname), offset >

/* ITM Trace Control Register Definitions */
ITM_BIT(TCR, 23) ITM_TCR_BUSY;
ITM_FIELD(TCR, 16, 7) ITM_TCR_ATBID;
ITM_FIELD(TCR, 8, 2) ITM_TCR_TSPrescale;
ITM_BIT(TCR, 4) ITM_TCR_SWOENA;
ITM_BIT(TCR, 3) ITM_TCR_DWTENA;
ITM_BIT(TCR, 2) ITM_TCR_SYNCENA;
ITM_BIT(TCR, 1) ITM_TCR_TSENA;
ITM_BIT(TCR, 0) ITM_TCR_ITMENA;

ITM_BIT(IWR, 0) ITM_IWR_ATVALIDM;
ITM_BIT(IRR, 0) ITM_IRR_ATREADYM;

/* ITM Integration Mode Control Register Definitions */
#define ITM_IMCR_INTEGRATION_Pos            0                                             /*!< ITM IMCR: INTEGRATION Position */
#define ITM_IMCR_INTEGRATION_Msk           (1UL << ITM_IMCR_INTEGRATION_Pos)              /*!< ITM IMCR: INTEGRATION Mask */

/* ITM Lock Status Register Definitions */
#define ITM_LSR_ByteAcc_Pos                 2                                             /*!< ITM LSR: ByteAcc Position */
#define ITM_LSR_ByteAcc_Msk                (1UL << ITM_LSR_ByteAcc_Pos)                   /*!< ITM LSR: ByteAcc Mask */

#define ITM_LSR_Access_Pos                  1                                             /*!< ITM LSR: Access Position */
#define ITM_LSR_Access_Msk                 (1UL << ITM_LSR_Access_Pos)                    /*!< ITM LSR: Access Mask */

#define ITM_LSR_Present_Pos                 0                                             /*!< ITM LSR: Present Position */
#define ITM_LSR_Present_Msk                (1UL << ITM_LSR_Present_Pos)                   /*!< ITM LSR: Present Mask */
}
/*@}*/ /* end of group CMSIS_ITM */
#endif // ifndef core_itm_h
