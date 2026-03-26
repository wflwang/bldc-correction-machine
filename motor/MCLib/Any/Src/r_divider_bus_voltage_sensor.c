/**
  ******************************************************************************
  * @file    r_driver_bus_voltage_sensor.c 
  *  @version V1.0.1
  * @date    2021-07-19

    @endverbatim
  *
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "r_divider_bus_voltage_sensor.h"
#include "parameters_conversion.h"
/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup BusVoltageSensor
  * @{
  */

/** @defgroup RDividerBusVoltageSensor Resistor Divider Bus Voltage Sensor
  * @brief Resistor Divider Bus Voltage Sensor implementation
  *
  * @todo Document the Resistor Divider Bus Voltage Sensor "module".
  *
  * @{
  */

/**
  * @brief  It initializes bus voltage conversion (ADC, ADC channel, conversion time. 
    It must be called only after PWMC_Init.
  * @param  pHandle related RDivider_Handle_t
  * @retval none
  */
void RVBS_Init( RDivider_Handle_t * pHandle )            
{
  /* Check */
  RVBS_Clear( pHandle );
}


/**
  * @brief  It clears  average bus voltage value        
  * @param  pHandle related RDivider_Handle_t
  * @retval none
  */
void RVBS_Clear( RDivider_Handle_t * pHandle )     
{
  uint16_t aux;
  uint16_t index;

  aux = ( pHandle->OverVoltageThreshold + pHandle->UnderVoltageThreshold ) / 2u;
  for ( index = 0u; index < pHandle->LowPassFilterBW; index++ )
  {
    pHandle->aBuffer[index] = aux;
  }
  pHandle->_Super.LatestConv = aux;
  pHandle->_Super.AvBusVoltage_d = aux;
  pHandle->index = 0;
}
/**
  * @brief  It configure the Vbus first order software filter to get Vbus average
  *         value
  * @param  pHandle related RDivider_Handle_t
  * @retval none
  */
static uint16_t RVBS_ConvertVbusFiltrered( RDivider_Handle_t * pHandle )  
{
  uint16_t hAux;
  uint8_t vindex;
  uint16_t max = 0, min = 0;
  uint32_t tot = 0u;

  for ( vindex = 0; vindex < pHandle->LowPassFilterBW; )     
  {
	hAux = M1_BUS_VOLTAGE_ADC_RESULT_REG;
    if ( hAux != 0xFFFFu )
    {
      if ( vindex == 0 )
      {
        min = hAux;
        max = hAux;
      }
      else
      {
        if ( hAux < min )
        {
          min = hAux;
        }
        if ( hAux > max )
        {
          max = hAux;
        }
      }
      vindex++;

      tot += hAux;
    }
  }

  tot -= max;
  tot -= min;
  return ( uint16_t )( tot / ( pHandle->LowPassFilterBW - 2u ) ); 
}

/**
  * @brief  It actually performes the Vbus ADC conversion and updates average
  *         value
  * @param  pHandle related RDivider_Handle_t
  * @retval uint16_t Fault code error
  */
uint16_t RVBS_CalcAvVbusFilt( RDivider_Handle_t * pHandle )
{
  uint32_t wtemp;
  uint16_t hAux;
  uint8_t i;

  hAux = RVBS_ConvertVbusFiltrered( pHandle );

  if ( hAux != 0xFFFF )
  {
    pHandle->aBuffer[pHandle->index] = hAux;
    wtemp = 0;
    for ( i = 0; i < pHandle->LowPassFilterBW; i++ )
    {
      wtemp += pHandle->aBuffer[i];
    }
    wtemp /= pHandle->LowPassFilterBW;
    pHandle->_Super.AvBusVoltage_d = ( uint16_t )wtemp;
    pHandle->_Super.LatestConv = hAux;

    if ( pHandle->index < pHandle->LowPassFilterBW - 1 )
    {
      pHandle->index++;
    }
    else
    {
      pHandle->index = 0;
    }
  }

  pHandle->_Super.FaultState = RVBS_CheckFaultState( pHandle );

  return ( pHandle->_Super.FaultState );
}

/**
  * @brief  It actually performes the Vbus ADC conversion and updates average
  *         value
  * @param  pHandle related RDivider_Handle_t
  * @retval uint16_t Fault code error
  */
uint16_t RVBS_CalcAvVbus( RDivider_Handle_t * pHandle )    
{
  uint32_t wtemp;
  uint32_t wtemp1;
  uint16_t hAux;
//  uint16_t hAux1;
  uint8_t i;

  hAux = M1_BUS_VOLTAGE_ADC_RESULT_REG; 
//  hAux1 = M1_IBUS_ADC_RESULT_REG&0xFFF; 
  if ( hAux != 0xFFFF )
  {
    pHandle->aBuffer[pHandle->index] = hAux;
//    pHandle->bBuffer[pHandle->index] = hAux1;
    wtemp = 0;
    for ( i = 0; i < pHandle->LowPassFilterBW; i++ )
    {
      wtemp += pHandle->aBuffer[i];
      wtemp1+= pHandle->bBuffer[i];
    }
    wtemp /= pHandle->LowPassFilterBW;
    wtemp1 /= pHandle->LowPassFilterBW;
    pHandle->_Super.AvBusVoltage_d = ( uint16_t )wtemp;
    pHandle->_Super.AvIBus_d = ( uint16_t )wtemp1;
    pHandle->_Super.LatestConv = hAux;
    pHandle->_Super.BusVoltage_mV = ((wtemp*ADC_REFERENCE_VOLTAGE/BUS_ADC_CONV_RATIO*1000)/65535);//V
    pHandle->_Super.IBusVoltage_mA = ((wtemp1*ADC_REFERENCE_VOLTAGE/RSHUNT*1000/AMPLIFICATION_GAIN_IBUS)/4095);//mA
    pHandle->_Super.Power_mW = (pHandle->_Super.BusVoltage_mV) *(pHandle->_Super.IBusVoltage_mA)/1000;
    if ( pHandle->index < pHandle->LowPassFilterBW - 1 )
    {
      pHandle->index++;
    }
    else
    {
      pHandle->index = 0;
    }
  }

  pHandle->_Super.FaultState = RVBS_CheckFaultState( pHandle );

  return ( pHandle->_Super.FaultState );
}

/**
  * @brief  It returns MC_OVER_VOLT, MC_UNDER_VOLT or MC_NO_ERROR depending on
  *         bus voltage and protection threshold values
  * @param  pHandle related RDivider_Handle_t
  * @retval uint16_t Fault code error
  */
uint16_t RVBS_CheckFaultState( RDivider_Handle_t * pHandle ) 
{
  uint16_t fault;

  if ( pHandle->_Super.AvBusVoltage_d > pHandle->OverVoltageThreshold )
  {
    fault = MC_OVER_VOLT;
  }
  else if ( pHandle->_Super.AvBusVoltage_d < pHandle->UnderVoltageThreshold )
  {
    fault = MC_UNDER_VOLT;
  }
  else
  {
    fault = MC_NO_ERROR;
  }
  return fault;
}


/**
  * @}
  */

/**
  * @}
  */

/**
   *@} 
	*/



