/*******************************************************************************
* @copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
* @filename:  hk32m07x_pwr.h
* @brief:     This file contains all the functions prototypes for the PWR
*             firmware library.
* @author:    AE Team
*******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HK32M07X_PWR_H
#define __HK32M07X_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup PWR  PWR
  * @brief PWR driver modules
  * @{
  */

/** @defgroup PWR_Exported_Constants PWR_Exported_Constants
  * @{
  */

#define  PWR_PVDLevel_0                     ((uint16_t)0x0002)     /*!< PVD level 0 */
#define  PWR_PVDLevel_1                     ((uint16_t)0x0003)     /*!< PVD level 1 */
#define  PWR_PVDLevel_2                     ((uint16_t)0x0004)     /*!< PVD level 2 */
#define  PWR_PVDLevel_3                     ((uint16_t)0x0005)     /*!< PVD level 3 */
#define  PWR_PVDLevel_4                     ((uint16_t)0x0006)     /*!< PVD level 4 */
#define  PWR_PVDLevel_5                     ((uint16_t)0x0007)     /*!< PVD level 5 */
#define  PWR_PVDLevel_6                     ((uint16_t)0x0008)     /*!< PVD level 6 */
#define  PWR_PVDLevel_7                     ((uint16_t)0x0009)     /*!< PVD level 7 */
#define  PWR_PVDLevel_8                     ((uint16_t)0x000A)     /*!< PVD level 8 */
#define  PWR_PVDLevel_9                     ((uint16_t)0x000B)     /*!< PVD level 9 */
#define  PWR_PVDLevel_10                     ((uint16_t)0x000C)    /*!< PVD level 10 */
#define  PWR_PVDLevel_11                     ((uint16_t)0x000D)     /*!< PVD level 11 */
#define  PWR_PVDLevel_12                     ((uint16_t)0x000E)     /*!< PVD level 12 */
#define  PWR_PVDLevel_13                     ((uint16_t)0x000F)     /*!< PVD level 13 */

#define IS_PWR_PVD_LEVEL(LEVEL) (((LEVEL) == PWR_PVDLevel_0) || ((LEVEL) == PWR_PVDLevel_1)|| \
                                 ((LEVEL) == PWR_PVDLevel_2) || ((LEVEL) == PWR_PVDLevel_3)|| \
                                 ((LEVEL) == PWR_PVDLevel_4) || ((LEVEL) == PWR_PVDLevel_5)|| \
                                 ((LEVEL) == PWR_PVDLevel_6) || ((LEVEL) == PWR_PVDLevel_7)|| \
                                 ((LEVEL) == PWR_PVDLevel_8) || ((LEVEL) == PWR_PVDLevel_9)|| \
                                 ((LEVEL) == PWR_PVDLevel_10) || ((LEVEL) == PWR_PVDLevel_11)|| \
                                 ((LEVEL) == PWR_PVDLevel_12) || ((LEVEL) == PWR_PVDLevel_13))


#define  PWR_BORLevel_0                     ((uint16_t)0x0002)     /*!< BOR level 0 */
#define  PWR_BORLevel_1                     ((uint16_t)0x0003)     /*!< BOR level 1 */
#define  PWR_BORLevel_2                     ((uint16_t)0x0004)     /*!< BOR level 2 */
#define  PWR_BORLevel_3                     ((uint16_t)0x0005)     /*!< BOR level 3 */
#define  PWR_BORLevel_4                     ((uint16_t)0x0006)     /*!< BOR level 4 */
#define  PWR_BORLevel_5                     ((uint16_t)0x0007)     /*!< BOR level 5 */
#define  PWR_BORLevel_6                     ((uint16_t)0x0008)     /*!< BOR level 6 */
#define  PWR_BORLevel_7                     ((uint16_t)0x0009)     /*!< BOR level 7 */
#define  PWR_BORLevel_8                     ((uint16_t)0x000A)     /*!< BOR level 8 */
#define  PWR_BORLevel_9                     ((uint16_t)0x000B)     /*!< BOR level 9 */
#define  PWR_BORLevel_10                     ((uint16_t)0x000C)    /*!< BOR level 10 */
#define  PWR_BORLevel_11                     ((uint16_t)0x000D)     /*!< BOR level 11 */
#define  PWR_BORLevel_12                     ((uint16_t)0x000E)     /*!< BOR level 12 */
#define  PWR_BORLevel_13                     ((uint16_t)0x000F)     /*!< BOR level 13 */

#define IS_PWR_BOR_LEVEL(LEVEL) (((LEVEL) == PWR_BORLevel_0) || ((LEVEL) == PWR_BORLevel_1)|| \
                                 ((LEVEL) == PWR_BORLevel_2) || ((LEVEL) == PWR_BORLevel_3)|| \
                                 ((LEVEL) == PWR_BORLevel_4) || ((LEVEL) == PWR_BORLevel_5)|| \
                                 ((LEVEL) == PWR_BORLevel_6) || ((LEVEL) == PWR_BORLevel_7)|| \
                                 ((LEVEL) == PWR_BORLevel_8) || ((LEVEL) == PWR_BORLevel_9)|| \
                                 ((LEVEL) == PWR_BORLevel_10) || ((LEVEL) == PWR_BORLevel_11)|| \
                                 ((LEVEL) == PWR_BORLevel_12) || ((LEVEL) == PWR_BORLevel_13))



#define PVD_MODE_IT               ((uint32_t)0x00010000U)
#define PVD_MODE_EVT              ((uint32_t)0x00020000U)

#define PVD_RISING_EDGE           ((uint32_t)0x00000001U)
#define PVD_FALLING_EDGE          ((uint32_t)0x00000002U)
#define PVD_RISING_FALLING        ((uint32_t)0x00000003U)

/**
  * @}
  */


/** @defgroup Regulator_state_is_STOP_mode Regulator_state_is_STOP_mode
  * @{
  */

#define PWR_Regulator_ON          ((uint32_t)0x00000000)
#define PWR_Regulator_LowPower    ((uint32_t)0x00000001)
#define IS_PWR_REGULATOR(REGULATOR) (((REGULATOR) == PWR_Regulator_LowPower)||((REGULATOR)==PWR_Regulator_ON))

/**
  * @}
  */

/** @defgroup PWR_STOP_mode_entry PWR_STOP_mode_entry
  * @{
  */

#define PWR_STOPEntry_WFI               ((uint8_t)0x01)
#define PWR_STOPEntry_WFE               ((uint8_t)0x02)
#define PWR_STOPEntry_SLEEPONEXIT       ((uint8_t)0x03)
#define IS_PWR_STOP_ENTRY(ENTRY) (((ENTRY) == PWR_STOPEntry_WFI) || ((ENTRY) == PWR_STOPEntry_WFE) ||\
                                  ((ENTRY) == PWR_STOPEntry_SLEEPONEXIT))

#define PWR_SLEEPEntry_WFI              ((uint8_t)0x01)
#define PWR_SLEEPEntry_WFE              ((uint8_t)0x02)
#define IS_PWR_SLEEP_ENTRY(ENTRY) (((ENTRY) == PWR_SLEEPEntry_WFI) || ((ENTRY) == PWR_SLEEPEntry_WFE))

/**
  * @}
  */

/** @defgroup PWR_mode_entry PWR_mode_entry
  * @{
  */

#define PWR_Entry_WFI              ((uint8_t)0x01)
#define PWR_Entry_WFE              ((uint8_t)0x02)
#define PWR_Entry_SLEEPONEXIT      ((uint8_t)0x03)
#define IS_PWR_ENTRY(ENTRY) (((ENTRY) == PWR_Entry_WFI) || ((ENTRY) == PWR_Entry_WFE) ||\
                                   ((ENTRY) == PWR_Entry_SLEEPONEXIT))

/**
  * @}
  */


/** @defgroup PWR_Flag  PWR_Flag
  * @{
  */

#define PWR_FLAG_LDORDY           ((uint32_t)0x00000008)
#define IS_PWR_GET_FLAG(FLAG) ((FLAG) == PWR_FLAG_LDORDY)

/**
  * @}
  */

/**
  * @}
  */


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* Function used to set the PWR configuration to the default reset state ******/
void PWR_DeInit(void);

/* Low Power modes configuration functions ************************************/

void PWR_EnterStopMode(uint32_t PWR_Regulator, uint8_t PWR_Entry);
void PWR_EnterSleepMode(uint8_t PWR_SLEEPEntry);
/* PVD/BOR configuration functions ********************************************/

void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel);
void PWR_PVDCmd(FunctionalState NewState);

#ifdef __cplusplus
}
#endif

#endif /*__HK32M07X_PWR_H */


