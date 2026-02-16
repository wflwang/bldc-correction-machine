/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_uart.h
* @brief:     UART initialization and configuration
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_UART_H
#define __HK32M07X_UART_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup UART UART
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup UART_Exported_Types UART_Exported_Types
  * @{
  */

/**
  * @brief  UART Init Structure definition
  */

typedef struct
{
    uint32_t UART_BaudRate;            /*!< This member configures the UART communication baud rate. */
    uint32_t UART_WordLength;          /*!< Specifies the number of data bits transmitted or received in a frame.
                                          This parameter can be a value of @ref UART_Word_Length */
    uint32_t UART_StopBits;            /*!< Specifies the number of stop bits transmitted.
                                          This parameter can be a value of @ref UART_Stop_Bits */
    uint32_t UART_Parity;              /*!< Specifies the parity mode.
                                          This parameter can be a value of @ref UART_Parity */
    uint32_t UART_Mode;                /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                          This parameter can be a value of @ref UART_Mode */
    FunctionalState UART_RESYN;        /*!< Specifies wether the resynchronization is enabled or disabled.
                                          This parameter can be set either to ENABLE or DISABLE. */
} UART_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup UART_Exported_Constants UART_Exported_Constants
  * @{
  */

/** @defgroup UART_Word_Length UART_Word_Length
  * @{
  */
#define UART_WordLength_8b                  ((uint32_t)0x00000000)
#define UART_WordLength_9b                  UART_CR1_M0
#define IS_UART_WORD_LENGTH(LENGTH) (((LENGTH) == UART_WordLength_8b) || ((LENGTH) == UART_WordLength_9b))
/**
  * @}
  */

/** @defgroup UART_Stop_Bits UART_Stop_Bits
  * @{
  */
#define UART_StopBits_1                     ((uint32_t)0x00000000)
#define UART_StopBits_2                     UART_CR2_STOP_1
#define IS_UART_STOPBITS(STOPBITS) (((STOPBITS) == UART_StopBits_1) || ((STOPBITS) == UART_StopBits_2))
/**
  * @}
  */

/** @defgroup UART_Parity UART_Parity
  * @{
  */
#define UART_Parity_No                      ((uint32_t)0x00000000)
#define UART_Parity_Even                    UART_CR1_PCE
#define UART_Parity_Odd                     (UART_CR1_PCE | UART_CR1_PS)
#define IS_UART_PARITY(PARITY) (((PARITY) == UART_Parity_No)    || \
                                ((PARITY) == UART_Parity_Even)  || \
                                ((PARITY) == UART_Parity_Odd))
/**
  * @}
  */

/** @defgroup UART_Mode UART_Mode
  * @{
  */
#define UART_Mode_Rx                        UART_CR1_RE
#define UART_Mode_Tx                        UART_CR1_TE
#define IS_UART_MODE(MODE) ((((MODE) & (uint32_t)0xFFFFFFF3) == 0x00) && ((MODE) != (uint32_t)0x00))
/**
  * @}
  */

/** @defgroup UART_MuteMode_WakeUp_methods UART_MuteMode_WakeUp_methods
  * @{
  */
#define UART_WakeUp_IdleLine                ((uint32_t)0x00000000)
#define UART_WakeUp_AddressMark             UART_CR1_WAKE
#define IS_UART_MUTEMODE_WAKEUP(WAKEUP) (((WAKEUP) == UART_WakeUp_IdleLine) || ((WAKEUP) == UART_WakeUp_AddressMark))
/**
  * @}
  */

/** @defgroup UART_Address_Detection UART_Address_Detection
  * @{
  */
#define UART_AddressLength_4b               ((uint32_t)0x00000000)
#define UART_AddressLength_7b               UART_CR2_ADDM7
#define IS_UART_ADDRESS_DETECTION(ADDRESS) (((ADDRESS) == UART_AddressLength_4b) || ((ADDRESS) == UART_AddressLength_7b))
/**
  * @}
  */

/** @defgroup UART_LIN_Break_Detection_Length UART_LIN_Break_Detection_Length
  * @{
  */
#define UART_LINBreakDetectLength_10b       ((uint32_t)0x00000000)
#define UART_LINBreakDetectLength_11b       UART_CR2_LBDL
#define IS_UART_LIN_BREAK_DETECT_LENGTH(LENGTH) (((LENGTH) == UART_LINBreakDetectLength_10b) || \
                                                 ((LENGTH) == UART_LINBreakDetectLength_11b))
/**
  * @}
  */

/** @defgroup UART_Inversion_Pins UART_Inversion_Pins
  * @{
  */
#define UART_InvPin_Tx                      UART_CR2_TXINV
#define UART_InvPin_Rx                      UART_CR2_RXINV
#define IS_UART_INVERSTION_PIN(PIN) ((((PIN) & (uint32_t)0xFFFCFFFF) == 0x00) && ((PIN) != (uint32_t)0x00))

/**
  * @}
  */

/** @defgroup UART_OVR_DETECTION UART_OVR_DETECTION
  * @{
  */
#define UART_OVRDetection_Enable            ((uint32_t)0x00000000)
#define UART_OVRDetection_Disable           UART_CR3_OVRDIS
#define IS_UART_OVRDETECTION(OVR) (((OVR) == UART_OVRDetection_Enable) || ((OVR) == UART_OVRDetection_Disable))
/**
  * @}
  */

/** @defgroup UART_Request UART_Request
  * @{
  */
#define UART_Request_SBKRQ                  UART_RQR_SBKRQ
#define UART_Request_MMRQ                   UART_RQR_MMRQ
#define UART_Request_RXFRQ                  UART_RQR_RXFRQ
#define IS_UART_REQUEST(REQUEST) (((REQUEST) == UART_Request_RXFRQ) || \
                                  ((REQUEST) == UART_Request_MMRQ)  || \
                                  ((REQUEST) == UART_Request_SBKRQ))
/**
  * @}
  */

/** @defgroup UART_Flags UART_Flags
  * @{
  */
#define UART_FLAG_REACK                     UART_ISR_REACK
#define UART_FLAG_TEACK                     UART_ISR_TEACK
#define UART_FLAG_RWU                       UART_ISR_RWU
#define UART_FLAG_SBK                       UART_ISR_SBKF
#define UART_FLAG_CM                        UART_ISR_CMF
#define UART_FLAG_BUSY                      UART_ISR_BUSY
#define UART_FLAG_LBDF                      UART_ISR_LBDF
#define UART_FLAG_TXE                       UART_ISR_TXE
#define UART_FLAG_TC                        UART_ISR_TC
#define UART_FLAG_RXNE                      UART_ISR_RXNE
#define UART_FLAG_IDLE                      UART_ISR_IDLE
#define UART_FLAG_ORE                       UART_ISR_ORE
#define UART_FLAG_NE                        UART_ISR_NF
#define UART_FLAG_FE                        UART_ISR_FE
#define UART_FLAG_PE                        UART_ISR_PE

#define IS_UART_FLAG(FLAG)  (((FLAG) == UART_FLAG_PE)   || ((FLAG) == UART_FLAG_TXE)   || \
                             ((FLAG) == UART_FLAG_TC)   || ((FLAG) == UART_FLAG_RXNE)  || \
                             ((FLAG) == UART_FLAG_IDLE) || ((FLAG) == UART_FLAG_ORE)   || \
                             ((FLAG) == UART_FLAG_NE)   || ((FLAG) == UART_FLAG_FE)    || \
                             ((FLAG) == UART_FLAG_BUSY) || ((FLAG) == UART_FLAG_LBDF)  || \
                             ((FLAG) == UART_FLAG_CM)   || ((FLAG) == UART_FLAG_SBK)   || \
                             ((FLAG) == UART_FLAG_RWU)  || ((FLAG) == UART_FLAG_TEACK) || \
                                           ((FLAG) == UART_FLAG_REACK))

#define IS_UART_CLEAR_FLAG(FLAG) (((FLAG) == UART_FLAG_PE)   || ((FLAG) == UART_FLAG_TC)  || \
                                  ((FLAG) == UART_FLAG_IDLE) || ((FLAG) == UART_FLAG_ORE) || \
                                  ((FLAG) == UART_FLAG_NE)   || ((FLAG) == UART_FLAG_FE)  || \
                                                    ((FLAG) == UART_FLAG_LBDF) ||   ((FLAG) == UART_FLAG_CM))
/**
  * @}
  */

/** @defgroup UART_Interrupt_definition UART_Interrupt_definition
  * @brief UART Interrupt definition
  * UART_IT possible values
  * Elements values convention: 0xZZZZYYXX
  *   XX: Position of the corresponding Interrupt
  *   YY: Register index
  *   ZZZZ: Flag position
  * @{
  */

#define UART_IT_ERR                         ((uint32_t)0x00000300)
#define UART_IT_CM                          ((uint32_t)0x0011010E)
#define UART_IT_LBD                         ((uint32_t)0x00080206)
#define UART_IT_TXE                         ((uint32_t)0x00070107)
#define UART_IT_TC                          ((uint32_t)0x00060106)
#define UART_IT_RXNE                        ((uint32_t)0x00050105)
#define UART_IT_IDLE                        ((uint32_t)0x00040104)
#define UART_IT_ORE                         ((uint32_t)0x00030300)
#define UART_IT_NE                          ((uint32_t)0x00020300)
#define UART_IT_FE                          ((uint32_t)0x00010300)
#define UART_IT_PE                          ((uint32_t)0x00000108)

#define IS_UART_CONFIG_IT(IT) (((IT) == UART_IT_PE)   || ((IT) == UART_IT_TXE)  || \
                               ((IT) == UART_IT_TC)   || ((IT) == UART_IT_RXNE) || \
                               ((IT) == UART_IT_IDLE) || ((IT) == UART_IT_LBD)  || \
                                               ((IT) == UART_IT_ERR)  || ((IT) == UART_IT_CM))

#define IS_UART_GET_IT(IT) (((IT) == UART_IT_PE)   || ((IT) == UART_IT_TXE)  || \
                            ((IT) == UART_IT_TC)   || ((IT) == UART_IT_RXNE) || \
                            ((IT) == UART_IT_IDLE) || ((IT) == UART_IT_NE)   || \
                            ((IT) == UART_IT_LBD)  || ((IT) == UART_IT_ORE)  || \
                                            ((IT) == UART_IT_ERR)  || ((IT) == UART_IT_CM))

#define IS_UART_CLEAR_IT(IT) (((IT) == UART_IT_PE)   || ((IT) == UART_IT_NE)  || \
                              ((IT) == UART_IT_TC)   || ((IT) == UART_IT_FE)  || \
                              ((IT) == UART_IT_IDLE) || ((IT) == UART_IT_LBD) || \
                              ((IT) == UART_IT_ORE)  || ((IT) == UART_IT_CM))
/**
  * @}
  */

/** @defgroup UART_Global_definition UART_Global_definition
  * @{
  */

#define IS_UART_ALL_PERIPH(PERIPH) (((PERIPH) == UART1) || ((PERIPH) == UART2))
#define IS_UART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x007A1201))
#define IS_UART_DATA(DATA) ((DATA) <= 0x1FF)
#define IS_UART_NF_LEVEL(VALUE)    ((VALUE) < 0x08)
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup UART_Exported_Functions UART_Exported_Functions
  * @{
  */
/* Initialization and Configuration functions *********************************/
void UART_DeInit(UART_TypeDef* UARTx);
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);
void UART_Cmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_DirectionModeCmd(UART_TypeDef* UARTx, uint32_t UART_DirectionMode, FunctionalState NewState);
void UART_OverSampling8Cmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_OneBitMethodCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_MSBFirstCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_DataInvCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_InvPinCmd(UART_TypeDef* UARTx, uint32_t UART_InvPin, FunctionalState NewState);
void UART_SWAPPinCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_RequestCmd(UART_TypeDef* UARTx, uint32_t UART_Request, FunctionalState NewState);
void UART_OverrunDetectionConfig(UART_TypeDef* UARTx, uint32_t UART_OVRDetection);
void UART_NFConfig(UART_TypeDef* UARTx, uint8_t NFLevel);
void UART_RESYNCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_HDUPECmd(UART_TypeDef* UARTx, FunctionalState NewState);

/* Data transfers functions ***************************************************/
void UART_SendData(UART_TypeDef* UARTx, uint16_t Data);
uint16_t UART_ReceiveData(UART_TypeDef* UARTx);

/* Multi-Processor Communication functions ************************************/
void UART_SetAddress(UART_TypeDef* UARTx, uint8_t UART_Address);
void UART_MuteModeWakeUpConfig(UART_TypeDef* UARTx, uint32_t UART_WakeUp);
void UART_MuteModeCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_AddressDetectionConfig(UART_TypeDef* UARTx, uint32_t UART_AddressLength);

/* Half-duplex mode function **************************************************/
void UART_HalfDuplexCmd(UART_TypeDef* UARTx, FunctionalState NewState);

/* LIN mode functions *********************************************************/
void UART_LINBreakDetectLengthConfig(UART_TypeDef* UARTx, uint32_t UART_LINBreakDetectLength);
void UART_LINCmd(UART_TypeDef* UARTx, FunctionalState NewState);
void UART_SendBreak(UART_TypeDef* UARTx);

/* Interrupts and flags management functions **********************************/
void UART_ITConfig(UART_TypeDef* UARTx, uint32_t UART_IT, FunctionalState NewState);
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint32_t UART_FLAG);
void UART_ClearFlag(UART_TypeDef* UARTx, uint32_t UART_FLAG);
ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint32_t UART_IT);
void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint32_t UART_IT);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__HK32M07X_UART_H */

/**
  * @}
  */



