/**
  ******************************************************************************
  * @file    virtual_bus_voltage_sensor.h 
  *  @version V1.0.1
  * @date    2021-07-23
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __VIRTUAL_BUSVOLTAGESENSOR_H
#define __VIRTUAL_BUSVOLTAGESENSOR_H

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

/** @addtogroup VirtualBusVoltageSensor
  * @{
  */

/**
  * @brief  Virtual Vbus sensor class parameters definition
  */
typedef struct
{
  BusVoltageSensor_Handle_t _Super;

  uint16_t ExpectedVbus_d;            /*!< Expected Vbus voltage expressed in
                                           digital value
                                           OverVoltageThreshold(digital value)=
                                           Over Voltage Threshold (V) * 65536
                                           / 500 */
} VirtualBusVoltageSensor_Handle_t;

/* Exported functions ------------------------------------------------------- */
void VVBS_Init( VirtualBusVoltageSensor_Handle_t * pHandle );
void VVBS_Clear( VirtualBusVoltageSensor_Handle_t * pHandle );
uint16_t VVBS_NoErrors( VirtualBusVoltageSensor_Handle_t * pHandle );

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

#endif /* __CCC_H */



