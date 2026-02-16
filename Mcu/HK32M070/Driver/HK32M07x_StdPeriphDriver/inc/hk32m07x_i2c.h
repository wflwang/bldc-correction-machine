/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_i2c.h
* @brief:     This file contains all the functions prototypes for the I2C
*             firmware library.
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_I2C_H
#define __HK32M07X_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"


/** @addtogroup I2C
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  I2C Init structure definition
  */

typedef struct
{
    uint32_t I2C_Timing;              /*!< Specifies the I2C_TIMINGR_register value.
                                         This parameter must be set by referring to I2C_Timing_Config_Tool*/

    uint32_t I2C_AnalogFilter;        /*!< Enables or disables analog noise filter.
                                         */

    uint32_t I2C_DigitalFilter;       /*!< Configures the digital noise filter.
                                         This parameter can be a number between 0x00 and 0x0F*/

    uint32_t I2C_Mode;                /*!< Specifies the I2C mode.
                                         */

    uint32_t I2C_OwnAddress1;         /*!< Specifies the device own address 1.
                                         This parameter can be a 7-bit or 10-bit address*/

    uint32_t I2C_Ack;                 /*!< Enables or disables the acknowledgement.
                                         */

    uint32_t I2C_AcknowledgedAddress; /*!< Specifies if 7-bit or 10-bit address is acknowledged.
                                         */
} I2C_InitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/

/** @defgroup I2C_Private_Defines I2C_Private_Defines
  * @{
   */
#define IS_I2C_ALL_PERIPH(PERIPH)       ((PERIPH) == I2C)

#define IS_I2C_1_PERIPH(PERIPH)         ((PERIPH) == I2C)


#define I2C_AnalogFilter_Enable         ((uint32_t)0x00000000)
#define I2C_AnalogFilter_Disable        I2C_CR1_ANFOFF

#define IS_I2C_ANALOG_FILTER(FILTER)    (((FILTER) == I2C_AnalogFilter_Enable) || \
                                         ((FILTER) == I2C_AnalogFilter_Disable))


#define IS_I2C_DIGITAL_FILTER(FILTER)   ((FILTER) <= 0x0000000F)


#define I2C_Mode_I2C                    ((uint32_t)0x00000000)

#define IS_I2C_MODE(MODE)               (((MODE) == I2C_Mode_I2C))


#define I2C_Ack_Enable                  ((uint32_t)0x00000000)
#define I2C_Ack_Disable                 I2C_CR2_NACK

#define IS_I2C_ACK(ACK)                 (((ACK) == I2C_Ack_Enable) || \
                                         ((ACK) == I2C_Ack_Disable))


#define I2C_AcknowledgedAddress_7bit    ((uint32_t)0x00000000)
#define I2C_AcknowledgedAddress_10bit   I2C_OAR1_OA1MODE

#define IS_I2C_ACKNOWLEDGE_ADDRESS(ADDRESS) (((ADDRESS) == I2C_AcknowledgedAddress_7bit) || \
                                             ((ADDRESS) == I2C_AcknowledgedAddress_10bit))


#define IS_I2C_OWN_ADDRESS1(ADDRESS1)   ((ADDRESS1) <= (uint32_t)0x000003FF)


#define I2C_Direction_Transmitter       ((uint16_t)0x0000)
#define I2C_Direction_Receiver          ((uint16_t)0x0400)

#define IS_I2C_DIRECTION(DIRECTION)     (((DIRECTION) == I2C_Direction_Transmitter) || \
                                         ((DIRECTION) == I2C_Direction_Receiver))

#define IS_I2C_SLAVE_ADDRESS(ADDRESS)   ((ADDRESS) <= (uint16_t)0x03FF)

#define IS_I2C_OWN_ADDRESS2(ADDRESS2)   ((ADDRESS2) <= (uint16_t)0x00FF)


#define I2C_OA2_NoMask                  ((uint8_t)0x00)
#define I2C_OA2_Mask01                  ((uint8_t)0x01)
#define I2C_OA2_Mask02                  ((uint8_t)0x02)
#define I2C_OA2_Mask03                  ((uint8_t)0x03)
#define I2C_OA2_Mask04                  ((uint8_t)0x04)
#define I2C_OA2_Mask05                  ((uint8_t)0x05)
#define I2C_OA2_Mask06                  ((uint8_t)0x06)
#define I2C_OA2_Mask07                  ((uint8_t)0x07)

#define IS_I2C_OWN_ADDRESS2_MASK(MASK)  (((MASK) == I2C_OA2_NoMask) || \
                                         ((MASK) == I2C_OA2_Mask01) || \
                                         ((MASK) == I2C_OA2_Mask02) || \
                                         ((MASK) == I2C_OA2_Mask03) || \
                                         ((MASK) == I2C_OA2_Mask04) || \
                                         ((MASK) == I2C_OA2_Mask05) || \
                                         ((MASK) == I2C_OA2_Mask06) || \
                                         ((MASK) == I2C_OA2_Mask07))


#define I2C_Register_CR1                ((uint8_t)0x00)
#define I2C_Register_CR2                ((uint8_t)0x04)
#define I2C_Register_OAR1               ((uint8_t)0x08)
#define I2C_Register_OAR2               ((uint8_t)0x0C)
#define I2C_Register_TIMINGR            ((uint8_t)0x10)
#define I2C_Register_TIMEOUTR           ((uint8_t)0x14)
#define I2C_Register_ISR                ((uint8_t)0x18)
#define I2C_Register_ICR                ((uint8_t)0x1C)
#define I2C_Register_RXDR               ((uint8_t)0x24)
#define I2C_Register_TXDR               ((uint8_t)0x28)

#define IS_I2C_REGISTER(REGISTER)       (((REGISTER) == I2C_Register_CR1) || \
                                         ((REGISTER) == I2C_Register_CR2) || \
                                         ((REGISTER) == I2C_Register_OAR1) || \
                                         ((REGISTER) == I2C_Register_OAR2) || \
                                         ((REGISTER) == I2C_Register_TIMINGR) || \
                                         ((REGISTER) == I2C_Register_TIMEOUTR) || \
                                         ((REGISTER) == I2C_Register_ISR) || \
                                         ((REGISTER) == I2C_Register_ICR) || \
                                         ((REGISTER) == I2C_Register_RXDR) || \
                                         ((REGISTER) == I2C_Register_TXDR))


#define I2C_IT_ERRI                     I2C_CR1_ERRIE
#define I2C_IT_TCI                      I2C_CR1_TCIE
#define I2C_IT_STOPI                    I2C_CR1_STOPIE
#define I2C_IT_NACKI                    I2C_CR1_NACKIE
#define I2C_IT_ADDRI                    I2C_CR1_ADDRIE
#define I2C_IT_RXI                      I2C_CR1_RXIE
#define I2C_IT_TXI                      I2C_CR1_TXIE

#define IS_I2C_CONFIG_IT(IT)            ((((IT) & (uint32_t)0xFFFFFF01) == 0x00) && ((IT) != 0x00))


#define I2C_FLAG_TXE                    I2C_ISR_TXE
#define I2C_FLAG_TXIS                   I2C_ISR_TXIS
#define I2C_FLAG_RXNE                   I2C_ISR_RXNE
#define I2C_FLAG_ADDR                   I2C_ISR_ADDR
#define I2C_FLAG_NACKF                  I2C_ISR_NACKF
#define I2C_FLAG_STOPF                  I2C_ISR_STOPF
#define I2C_FLAG_TC                     I2C_ISR_TC
#define I2C_FLAG_TCR                    I2C_ISR_TCR
#define I2C_FLAG_BERR                   I2C_ISR_BERR
#define I2C_FLAG_ARLO                   I2C_ISR_ARLO
#define I2C_FLAG_OVR                    I2C_ISR_OVR
#define I2C_FLAG_BUSY                   I2C_ISR_BUSY

#define IS_I2C_CLEAR_FLAG(FLAG)         ((((FLAG) & (uint32_t)0xFFFF4000) == 0x00) && ((FLAG) != 0x00))

#define IS_I2C_GET_FLAG(FLAG)           (((FLAG) == I2C_FLAG_TXE) || ((FLAG) == I2C_FLAG_TXIS) || \
                                         ((FLAG) == I2C_FLAG_RXNE) || ((FLAG) == I2C_FLAG_ADDR) || \
                                         ((FLAG) == I2C_FLAG_NACKF) || ((FLAG) == I2C_FLAG_STOPF) || \
                                         ((FLAG) == I2C_FLAG_TC) || ((FLAG) == I2C_FLAG_TCR) || \
                                         ((FLAG) == I2C_FLAG_BERR) || ((FLAG) == I2C_FLAG_ARLO) || \
                                         ((FLAG) == I2C_FLAG_OVR) || ((FLAG) == I2C_FLAG_BUSY))


#define I2C_IT_TXIS                     I2C_ISR_TXIS
#define I2C_IT_RXNE                     I2C_ISR_RXNE
#define I2C_IT_ADDR                     I2C_ISR_ADDR
#define I2C_IT_NACKF                    I2C_ISR_NACKF
#define I2C_IT_STOPF                    I2C_ISR_STOPF
#define I2C_IT_TC                       I2C_ISR_TC
#define I2C_IT_TCR                      I2C_ISR_TCR
#define I2C_IT_BERR                     I2C_ISR_BERR
#define I2C_IT_ARLO                     I2C_ISR_ARLO
#define I2C_IT_OVR                      I2C_ISR_OVR

#define IS_I2C_CLEAR_IT(IT)             ((((IT) & (uint32_t)0xFFFFC001) == 0x00) && ((IT) != 0x00))

#define IS_I2C_GET_IT(IT)               (((IT) == I2C_IT_TXIS) || ((IT) == I2C_IT_RXNE) || \
                                         ((IT) == I2C_IT_ADDR) || ((IT) == I2C_IT_NACKF) || \
                                         ((IT) == I2C_IT_STOPF) || ((IT) == I2C_IT_TC) || \
                                         ((IT) == I2C_IT_TCR) || ((IT) == I2C_IT_BERR) || \
                                         ((IT) == I2C_IT_ARLO) || ((IT) == I2C_IT_OVR))


#define I2C_Reload_Mode                 I2C_CR2_RELOAD
#define I2C_AutoEnd_Mode                I2C_CR2_AUTOEND
#define I2C_SoftEnd_Mode                ((uint32_t)0x00000000)


#define IS_RELOAD_END_MODE(MODE)        (((MODE) == I2C_Reload_Mode) || \
                                         ((MODE) == I2C_AutoEnd_Mode) || \
                                         ((MODE) == I2C_SoftEnd_Mode))



#define I2C_No_StartStop                ((uint32_t)0x00000000)
#define I2C_Generate_Stop               I2C_CR2_STOP
#define I2C_Generate_Start_Read         (uint32_t)(I2C_CR2_START | I2C_CR2_RD_WRN)
#define I2C_Generate_Start_Write        I2C_CR2_START

#define IS_START_STOP_MODE(MODE)        (((MODE) == I2C_Generate_Stop) || \
                                         ((MODE) == I2C_Generate_Start_Read) || \
                                         ((MODE) == I2C_Generate_Start_Write) || \
                                         ((MODE) == I2C_No_StartStop))

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Initialization and Configuration functions *********************************/
void I2C_DeInit(I2C_TypeDef* I2Cx);
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct);
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct);
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx);
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint32_t I2C_IT, FunctionalState NewState);
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_DualAddressCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint16_t Address, uint8_t Mask);
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_SlaveByteControlCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_SlaveAddressConfig(I2C_TypeDef* I2Cx, uint16_t Address);
void I2C_TenBitAddressingModeCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_SWAPPinCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

/* Communications handling functions ******************************************/
void I2C_AutoEndCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_ReloadCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_NumberOfBytesConfig(I2C_TypeDef* I2Cx, uint8_t Number_Bytes);
void I2C_MasterRequestConfig(I2C_TypeDef* I2Cx, uint16_t I2C_Direction);
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_TenBitAddressHeaderCmd(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState);
uint8_t I2C_GetAddressMatched(I2C_TypeDef* I2Cx);
uint16_t I2C_GetTransferDirection(I2C_TypeDef* I2Cx);
void I2C_TransferHandling(I2C_TypeDef* I2Cx, uint16_t Address, uint8_t Number_Bytes, uint32_t ReloadEndMode, uint32_t StartStopMode);

/* I2C registers management functions *****************************************/
uint32_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register);

/* Data transfers management functions ****************************************/
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx);

/* Interrupts and flags management functions **********************************/
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
void I2C_ClearFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG);
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT);
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, uint32_t I2C_IT);


#ifdef __cplusplus
}
#endif

#endif /*__HK32M07X_I2C_H */


