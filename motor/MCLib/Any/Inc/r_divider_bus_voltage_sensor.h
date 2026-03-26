/**
  ******************************************************************************
  * @file    r_divider_bus_voltage_sensor.h 
  *  @version V1.0.1
  * @date    2021-07-23
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RDIVIDER_BUSVOLTAGESENSOR_H
#define __RDIVIDER_BUSVOLTAGESENSOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "bus_voltage_sensor.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup BusVoltageSensor
  * @{
  */

/** @addtogroup RDividerBusVoltageSensor
  * @{
  */


/**
  * @brief  Rdivider class parameters definition
  */
typedef struct
{
  BusVoltageSensor_Handle_t _Super;

  uint16_t       LowPassFilterBW;       /*!< Use this number to configure the Vbus
                                             first order software filter bandwidth.
                                              */
  uint16_t       OverVoltageThreshold;  /*!< It represents the over voltage protection
                                             intervention threshold. To be expressed
                                             in digital value through formula:
                                             OverVoltageThreshold (digital value) =
                                             Over Voltage Threshold (V) * 65536
                                             / ConversionFactor */
  uint16_t       UnderVoltageThreshold; /*!< It represents the under voltage protection
                                             intervention threshold. To be expressed
                                             in digital value through formula:
                                             UnderVoltageThreshold (digital value)=
                                             Under Voltage Threshold (V) * 65536
                                             / ConversionFactor */
  uint16_t       *aBuffer;                /*!< Buffer used to compute average value.*/
  uint16_t       *bBuffer;                /*!< Buffer used to compute average value.*/
  uint8_t        elem;                  /*!< Number of stored elements in the average buffer.*/
  uint8_t        index;                 /*!< Index of last stored element in the average buffer.*/
//  uint8_t        convHandle;            /*!< handle to the regular conversion */

} RDivider_Handle_t;

/* Exported functions ------------------------------------------------------- */
void RVBS_Init( RDivider_Handle_t * pHandle );
void RVBS_Clear( RDivider_Handle_t * pHandle );
uint16_t RVBS_CalcAvVbusFilt( RDivider_Handle_t * pHandle );
uint16_t RVBS_CalcAvVbus( RDivider_Handle_t * pHandle );
uint16_t RVBS_CheckFaultState( RDivider_Handle_t * pHandle );

/**
  * @}
  */

/**
  * @}
  */

/**
  * @} 
  */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* __RDividerBusVoltageSensor_H */



