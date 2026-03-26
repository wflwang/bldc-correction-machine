/**
  ******************************************************************************
  * @file    motorcontrol.c
  * @version V1.0.0
  * @date    2021-07-28
  ******************************************************************************
  */
#include "main.h"
#include "mc_tuning.h"
#include "mc_interface.h"

#include "mc_tasks.h"
#include "mc_config.h"
#include "motorcontrol.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup MCAPI
  * @{
  */

MCI_Handle_t* pMCI[NBR_OF_MOTORS];
MCT_Handle_t* pMCT[NBR_OF_MOTORS];

/**
 * @brief Initializes and configures the Motor Control Subsystem
 *
 *  This function initializes and configures all the structures and components needed
 * for the Motor Control subsystem required by the Application. It expects that
 * all the peripherals needed for Motor Control purposes are already configured but
 * that their interrupts are not enabled yet.
 *
 * CubeMX calls this function after all peripherals initializations and
 * before the NVIC is configured
 */
void MX_MotorControl_Init(void)
{
    /* Reconfigure the SysTick interrupt to fire every 500 us. */
    SysTick_Config(SystemCoreClock / SYS_TICK_FREQUENCY);
    /* Initialize the Motor Control Subsystem */
    MCboot(pMCI, pMCT);
    mc_lock_pins();
}

/**
  * @}
  */

/**
  * @}
  */

