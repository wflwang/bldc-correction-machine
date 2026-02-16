/********************************************************************************
  * @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd
  * @filename: hk32m07x_flash.c
  * @brief: FLASH Initialization and Configuration functions
  * @Author: AE Team
  * @version: V1.0.0/2022-05-24
*********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_flash.h"

/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup FLASH FLASH
  * @brief FLASH driver modules
  * @{

    @verbatim
 ===============================================================================
                    ##### How to use this driver #####
 ===============================================================================
    [..] This driver provides functions to configure and program the Flash
         memory of all hk32m07x devices. These functions are split in 4 groups
         (#) FLASH Interface configuration functions: this group includes the
             management of following features:
             (++) Set the latency
             (++) Enable/Disable the prefetch buffer

         (#) FLASH Memory Programming functions: this group includes all needed
             functions to erase and program the main memory:
             (++) Lock and Unlock the Flash interface.
             (++) Erase function: Erase Page, erase all pages.
             (++) Program functions: Half Word and Word write.

         (#) FLASH Option Bytes Programming functions: this group includes all
             needed functions to:
             (++) Lock and Unlock the Flash Option bytes.
             (++) Launch the Option Bytes loader
             (++) Erase the Option Bytes
             (++)Set/Reset the write protection
             (++) Set the Read protection Level
             (++) Program the user option Bytes
             (++) Set/Reset the BOOT1 bit
             (++) Enable/Disable the VDDA Analog Monitoring
             (++) Get the user option bytes
             (++) Get the Write protection
             (++) Get the read protection status

         (#) FLASH Interrupts and flag management functions: this group includes
             all needed functions to:
             (++) Enable/Disable the flash interrupt sources
             (++) Get flags status
             (++) Clear flags
             (++) Get Flash operation status
             (++) Wait for last flash operation

 @endverbatim
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup FLASH_Private_Functions FLASH_Private_Functions
  * @{
  */

/** @defgroup FLASH_Group1 FLASH Interface configuration functions
  *  @brief   FLASH Interface configuration functions
 *
@verbatim
 ===============================================================================
               ##### FLASH Interface configuration functions #####
 ===============================================================================

    [..] FLASH_Interface configuration_Functions, includes the following functions:
       (+) void FLASH_SetLatency(uint32_t FLASH_Latency):
    [..] To correctly read data from Flash memory, the number of wait states (LATENCY)
     must be correctly programmed according to the frequency of the CPU clock (HCLK)
    [..]
        +-------------------------------------------------------------- +
        |---------------|-----------------------------------------------|
        |0WS(1CPU cycle)|             0 < HCLK <= 24                    |
        |---------------|-----------------------------------------------|
        |1WS(2CPU cycle)|            24 < HCLK <= 48                    |
        |---------------|-----------------------------------------------|
        |2WS(3CPU cycle)|            48 < HCLK <= 64                    |
        +---------------------------------------------------------------+
    [..]
       (+) void FLASH_PrefetchBufferCmd(FunctionalState NewState);
    [..]
     All these functions don't need the unlock sequence.

@endverbatim
  * @{
  */

/**
  * @brief  Sets the code latency value.
  * @note   None
  * @param  FLASH_Latency: specifies the FLASH Latency value.
  *          This parameter can be one of the following values:
  *             @arg FLASH_Latency_0: FLASH Zero Latency cycle
  *             @arg FLASH_Latency_1: FLASH One Latency cycle
  *             @arg FLASH_Latency_2: FLASH TWO Latency cycles
  *             @arg FLASH_Latency_3: FLASH THREE Latency cycles
  *             @arg FLASH_Latency_4: FLASH SEVEN Latency cycles
  *             @arg FLASH_Latency_5: FLASH NINE Latency cycles
  *             @arg FLASH_Latency_6: FLASH NINETEEN Latency cycles
  *             @arg FLASH_Latency_7: FLASH THIRTY-NINE Latency cycles
  * @retval None
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
    uint32_t tmpreg = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_LATENCY(FLASH_Latency));

    /* Read the ACR register */
    tmpreg = FLASH->ACR;

    /* Sets the Latency value */
    tmpreg &= (uint32_t) (~((uint32_t)FLASH_ACR_LATENCY));
    tmpreg |= FLASH_Latency;

    /* Write the ACR register */
    FLASH->ACR = tmpreg;
}

/**
  * @brief  Enables or disables the Prefetch Buffer.
  * @note   None
  * @param  NewState: new state of the FLASH prefetch buffer.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void FLASH_PrefetchBufferCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        FLASH->ACR |= FLASH_ACR_PRFTBE;
    }
    else
    {
        FLASH->ACR &= (uint32_t)(~((uint32_t)FLASH_ACR_PRFTBE));
    }
}

/**
  * @brief  Checks whether the FLASH Prefetch Buffer status is set or not.
  * @note   None
  * @retval FLASH Prefetch Buffer Status (SET or RESET).
  */
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
    FlagStatus bitstatus = RESET;

    if ((FLASH->ACR & FLASH_ACR_PRFTBS) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the new state of FLASH Prefetch Buffer Status (SET or RESET) */
    return bitstatus;
}

/**
  * @}
  */

/** @defgroup FLASH_Group2 FLASH Memory Programming functions
 *  @brief   FLASH Memory Programming functions
 *  @note   None
 *
@verbatim
 ===============================================================================
                ##### FLASH Memory Programming functions #####
 ===============================================================================

    [..] The FLASH Memory Programming functions, includes the following functions:
       (+) void FLASH_Unlock(void);
       (+) void FLASH_Lock(void);
       (+) FLASH_Status FLASH_ErasePage(uint32_t Page_Address);
       (+) FLASH_Status FLASH_EraseAllPages(void);
       (+) FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data);
       (+) FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data);

    [..] Any operation of erase or program should follow these steps:

       (#) Call the FLASH_Unlock() function to enable the flash control register and
           program memory access
       (#) Call the desired function to erase page or program data
       (#) Call the FLASH_Lock() to disable the flash program memory access
      (recommended to protect the FLASH memory against possible unwanted operation)

@endverbatim
  * @{
  */

/**
  * @brief  Unlocks the FLASH control register and program memory access.
  * @note   None
  * @retval None
  */
void FLASH_Unlock(void)
{
    if ((FLASH->CR & FLASH_CR_LOCK) != RESET)
    {
        /* Unlocking the program memory access */
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

/**
  * @brief  Locks the Program memory access.
  * @note   None
  * @retval None
  */
void FLASH_Lock(void)
{
    /* Set the LOCK Bit to lock the FLASH control register and program memory access */
    FLASH->CR |= FLASH_CR_LOCK;
}

/**
  * @brief  Unlocks the FLASH INFO control register and program memory access.
  * @note   None
  * @retval None
  */
void FLASH_InfoUnlock(void)
{

    /* Unlocking the FLASH INFO Area program memory access */
    uint32_t UID0_Data = *((volatile uint32_t *)(0x1ffffb00)) ;
    uint32_t UID1_Data = *((volatile uint32_t *)(0x1ffffb04)) ;
    uint32_t UID0_Tmp= UID0_Data ^ (UID0_Data<<1);
    uint32_t UID1_Tmp= UID1_Data ^ (UID1_Data>>1);
 
    FLASH->INFOKEYR = UID0_Tmp; 
    FLASH->INFOKEYR = UID1_Tmp;  
}

/**
  * @brief  Unlocks the FLASH INFO control register and program memory access.
  * @note   None
  * @retval None
  */
void FLASH_InfoLock(void)
{ 
   FLASH->INFOCSR |= FLASH_INFOCSR_INFO_LOCK; 
}

/**
  * @brief  Unlocks the FLASH INFO control register and program memory access.
  * @note   None
  * @retval None
  */
FlagStatus FLASH_GetInfoLockStatus(void)
{
    return (FlagStatus)(FLASH->INFOCSR & FLASH_INFOCSR_INFO_LOCK);
}

/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @note   A Page is erased in the Program memory only if the address to load
  *         is the start address of a page (multiple of 512 bytes).
  * @param  Page_Address: The page address in program memory to be erased.
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASHINFO_ErasePage(uint32_t Page_Address)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_FLASH_INFO_ADDRESS(Page_Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        /* If the previous operation is completed, proceed to erase the page */
        FLASH->ECR = ((((uint32_t)FLASH_INFO_ROPTKEY)<<16)|FLASH_ECR_INFO_PER);  
       
        FLASH->AR  = Page_Address;
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
 
        FLASH->ECR &= ~((((uint32_t)FLASH_INFO_ROPTKEY)<<16)|FLASH_ECR_INFO_PER);
    }

    /* Return the Erase Status */
    return status;
}


/**
  * @brief  Programs a halfword at a specified address.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASHINFO_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_INFO_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        FLASH->ECR = (((uint32_t)FLASH_INFO_ROPTKEY)<<16)|FLASH_ECR_INFO_PG; 
        *(__IO uint16_t*)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        FLASH->ECR &= ~((((uint32_t)FLASH_INFO_ROPTKEY)<<16)|FLASH_ECR_INFO_PG);

    }

    /* Return the Program Status */
    return status;
}


/**
  * @brief  Programs a word at a specified address.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASHINFO_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_INFO_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        FLASH->ECR = (((uint32_t)FLASH_INFO_ROPTKEY)<<16)|FLASH_ECR_INFO_WPG;   
        
        *(__IO uint32_t*)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        FLASH->ECR &= ~((((uint32_t)FLASH_INFO_ROPTKEY)<<16)|FLASH_ECR_INFO_WPG);

    }

    /* Return the Program Status */
    return status;
}


/**
  * @brief  Erases a specified page in program memory.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @note   A Page is erased in the Program memory only if the address to load
  *         is the start address of a page (multiple of 512 bytes).
  * @param  Page_Address: The page address in program memory to be erased.
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_FLASH_PROGRAM_ADDRESS(Page_Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        /* If the previous operation is completed, proceed to erase the page */
        FLASH->CR |= FLASH_CR_PER;
        FLASH->AR  = Page_Address;
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        /* Disable the PER Bit */
        FLASH->CR &= ~FLASH_CR_PER;
    }

    /* Return the Erase Status */
    return status;
}

/**
  * @brief  Erases all FLASH pages.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        /* if the previous operation is completed, proceed to erase all pages */
        FLASH->CR |= FLASH_CR_MER;
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        /* Disable the MER Bit */
        FLASH->CR &= ~FLASH_CR_MER;
    }

    /* Return the Erase Status */
    return status;
}

/**
  * @brief  Programs a halfword at a specified address.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {

        FLASH->CR |= FLASH_CR_PG;

        *(__IO uint16_t*)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        /* Disable the PG Bit */
        FLASH->CR &= ~FLASH_CR_PG;

    }

    /* Return the Program Status */
    return status;
}


/**
  * @brief  Programs a word at a specified address.
  * @note   To correctly run this function, the FLASH_Unlock() function must be called before.
  * @note   Call the FLASH_Lock() to disable the flash memory access (recommended
  *         to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;
    __IO uint32_t tmp = 0;

    /* Check the parameters */
    assert_param(IS_FLASH_PROGRAM_ADDRESS(Address));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {

        FLASH->ECR |= FLASH_ECR_WPG;

        *(__IO uint32_t*)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        /* Disable the WPG Bit */
        FLASH->ECR &= ~FLASH_ECR_WPG;

    }

    /* Return the Program Status */
    return status;
}

/**
  * @}
  */

/** @defgroup FLASH_Group3 Option Bytes Programming functions
 *  @brief   Option Bytes Programming functions
 *
@verbatim
 ===============================================================================
                ##### Option Bytes Programming functions #####
 ===============================================================================

    [..] The FLASH_Option Bytes Programming_functions, includes the following functions:
       (+) void FLASH_OBUnlock(void);
       (+) void FLASH_OBLock(void);
       (+) void FLASH_OBLaunch(void);
       (+) FLASH_Status FLASH_OBErase(void);
       (+) FLASH_Status FLASH_OBWRPConfig(uint32_t OB_WRP, FunctionalState NewState);
       (+) FLASH_Status FLASH_OBRDPConfig(uint8_t OB_RDP);
       (+) FLASH_Status FLASH_OBWriteUser(uint8_t OB_USER);
       (+) FLASH_OB_ProgramData(uint32_t Address, uint8_t Data);
       (+) uint8_t FLASH_OBGetUser(void);
       (+) uint32_t FLASH_OBGetWRP(void);
       (+) FlagStatus FLASH_OBGetRDP(void);

    [..] Any operation of erase or program should follow these steps:

   (#) Call the FLASH_OB_Unlock() function to enable the Option Bytes registers access

   (#) Call one or several functions to program the desired option bytes
      (++) FLASH_Status FLASH_OB_RDPConfig(uint8_t OB_RDP) => to set the desired read Protection Level
      (++) FLASH_Status FLASH_OB_WRPConfig(uint32_t OB_WRP, FunctionalState NewState)
           => to Enable/Disable the desired sector write protection
      (++) FLASH_Status FLASH_OB_UserConfig(uint8_t OB_IWDG, uint8_t OB_STOP, uint8_t OB_STDBY)
           => to configure the user option Bytes: IWDG, STOP and the Standby.
      (++) FLASH_Status FLASH_OB_VDDAConfig(uint8_t OB_VDDA_ANALOG)
           => to enable or disable the VDDA Analog Monitoring
      (++) You can write all User Options bytes at once using a single function
           by calling FLASH_Status FLASH_OB_WriteUser(uint8_t OB_USER)
      (++) FLASH_OB_ProgramData(uint32_t Address, uint8_t Data) to program the
           two half word in the option bytes

   (#) Once all needed option bytes to be programmed are correctly written, call the
      FLASH_OB_Launch(void) function to launch the Option Bytes programming process.

   (#) Call the FLASH_OB_Lock() to disable the Option Bytes registers access (recommended
      to protect the option Bytes against possible unwanted operations)

@endverbatim
  * @{
  */

/**
  * @brief  Unlocks the option bytes block access.
  * @note   None
  * @retval None
  */
void FLASH_OBUnlock(void)
{
    if ((FLASH->CR & FLASH_CR_OPTWRE) == RESET)
    {
        /* Unlocking the option bytes block access */
        FLASH->OPTKEYR = FLASH_OPTKEY1;
        FLASH->OPTKEYR = FLASH_OPTKEY2;
    }
}

/**
  * @brief  Locks the option bytes block access.
  * @note   None
  * @retval None
  */
void FLASH_OBLock(void)
{
    /* Set the OPTWREN Bit to lock the option bytes block access */
    FLASH->CR &= ~FLASH_CR_OPTWRE;

}

/**
  * @brief  Launch the option byte loading.
  * @note   None
  * @retval None
  */
void FLASH_OBLaunch(void)
{
    /* Set the OBL_Launch bit to launch the option byte loading */
    FLASH->CR |= FLASH_CR_OBL_LAUNCH;
}

/**
  * @brief  Erases the FLASH option bytes.
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @note   This functions erases all option bytes except the Read protection (RDP).
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBErase(void)
{
    uint16_t rdptmp = OB_RDP_Level_0;

    FLASH_Status status = FLASH_COMPLETE;

    /* Get the actual read protection Option Byte value */
    if (FLASH_OBGetRDP() != RESET)
    {
        rdptmp = 0x00;
    }

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        /* If the previous operation is completed, proceed to erase the option bytes */
        FLASH ->AR = 0x1FFFF800U;
        FLASH->CR |= FLASH_CR_OPTER;
        FLASH->CR |= FLASH_CR_STRT;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        if (status == FLASH_COMPLETE)
        {
            /* If the erase operation is completed, disable the OPTER Bit */
            FLASH->CR &= ~FLASH_CR_OPTER;

            /* Enable the Option Bytes Programming operation */
            FLASH->CR |= FLASH_CR_OPTPG;

            /* Restore the last read protection Option Byte value */
            OB_RDP_REG = (uint16_t)rdptmp;

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

            if (status != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CR &= ~FLASH_CR_OPTPG;
            }
        }
        else
        {
            if (status != FLASH_TIMEOUT)
            {
                /* Disable the OPTPG Bit */
                FLASH->CR &= ~FLASH_CR_OPTPG;
            }
        }
    }

    /* Return the erase status */
    return status;
}

/**
  * @brief  Write protects the desired pages
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  OB_WRP: specifies the address of the pages to be write protected.
  *          This parameter can be:
  *             @arg OB_WRP_Pages0to3..OB_WRP_Pages60to63
  *             @arg OB_WRP_AllPages
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBEnableWRP(uint32_t OB_WRP)
{
    uint32_t WRP01_Data = 0xFFFFFFFF, WRP23_Data = 0xFFFFFFFF;

    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_OB_WRP(OB_WRP));

    OB_WRP = (uint32_t)(~OB_WRP);
    WRP01_Data = (uint32_t)((OB_WRP & OB_WRP0_WRP0) | (((OB_WRP >> 8) & OB_WRP0_WRP0) << 16));
    WRP23_Data = (uint32_t)(((OB_WRP >> 16) & OB_WRP0_WRP0) | (((OB_WRP >> 24) & OB_WRP0_WRP0) << 16));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        FLASH->CR |= FLASH_CR_OPTPG;

        if (WRP01_Data != 0xFF00FF)
        {
            OB_WRP0_REG = (uint16_t)WRP01_Data;
			OB_WRP1_REG = (uint16_t)(WRP01_Data>>16);
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
        }

        if ((status == FLASH_COMPLETE) && (WRP23_Data != 0xFF00FF))
        {
            OB_WRP2_REG = (uint16_t)WRP23_Data;
			OB_WRP3_REG = (uint16_t)(WRP23_Data>>16);
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
        }

        if (status != FLASH_TIMEOUT)
        {
            /* if the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the write protection operation Status */
    return status;
}

/**
  * @brief  Enables or disables the read out protection.
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  OB_RDP: specifies the read protection level.
  *          This parameter can be:
  *             @arg OB_RDP_Level_0: No protection
  *             @arg OB_RDP_Level_1: Read protection of the memory
  *             @arg OB_RDP_Level_2: Chip protection
  * @note   When enabling OB_RDP level 2 it's no more possible to go back to level 1 or 0
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBRDPConfig(uint16_t OB_RDP)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_OB_RDP(OB_RDP));
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {  
        /* If the previous operation is completed, proceed to erase the option bytes */
        FLASH ->AR = 0x1FFFF800U;
        FLASH->CR |= FLASH_CR_OPTER;
        FLASH->CR |= FLASH_CR_STRT;
	
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        if (status == FLASH_COMPLETE)
        {
            /* If the erase operation is completed, disable the OPTER Bit */
            FLASH->CR &= ~FLASH_CR_OPTER;

            /* Enable the Option Bytes Programming operation */
            FLASH->CR |= FLASH_CR_OPTPG;

            OB_RDP_REG = OB_RDP;

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

            if (status != FLASH_TIMEOUT)
            {
                /* if the program operation is completed, disable the OPTPG Bit */
                FLASH->CR &= ~FLASH_CR_OPTPG;
            }
        }
        else
        {
            if (status != FLASH_TIMEOUT)
            {
                /* Disable the OPTER Bit */
                FLASH->CR &= ~FLASH_CR_OPTER;
            }
        }
    }

    /* Return the protection operation Status */
    return status;
}


/**
  * @brief  Programs the FLASH User Option Byte: IWDG_SW .
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  OB_IWDG: Selects the WDG mode
  *          This parameter can be one of the following values:
  *             @arg OB_IWDG_SW: Software WDG selected
  *             @arg OB_IWDG_HW: Hardware WDG selected
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBIWDGConfig(uint8_t OB_IWDG)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check the parameters */
    assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        /* Enable the Option Bytes Programming operation */
        FLASH->CR |= FLASH_CR_OPTPG;

        OB_USER_REG = (uint16_t)(OB_IWDG | 0xFE);

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte program Status */
    return status;
}


/**
  * @brief  Programs a  word at a specified Option Byte Data address.
  * @param  BOR_LEVEL: BOR Level
  *          This parameter can be  OB_BOR_LEVEL_DISABLE
    *        @arg OB_BOR_LEVEL_1
    *        @arg OB_BOR_LEVEL_2
    *        @arg OB_BOR_LEVEL_3
    *        @arg OB_BOR_LEVEL_4
    *        @arg OB_BOR_LEVEL_5
    *        @arg OB_BOR_LEVEL_6
    *        @arg OB_BOR_LEVEL_7
    *        @arg OB_BOR_LEVEL_8
    *        @arg OB_BOR_LEVEL_9
    *        @arg OB_BOR_LEVEL_10
    *        @arg OB_BOR_LEVEL_11
    *        @arg OB_BOR_LEVEL_12
    *        @arg OB_BOR_LEVEL_13
    *        @arg OB_BOR_LEVEL_14
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBBORLEVEL(uint8_t BOR_LEVEL)
{
	/* Check the parameters */
	assert_param(IS_OB_BOR(BOR_LEVEL));
	
    uint16_t borlevel = BOR_LEVEL | (0x00 << 8);
    uint16_t bor = *(uint16_t *)FLASH_OB_BOR_LEVEL_ADDRESS;

    FLASH_Status status = FLASH_COMPLETE;
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        bor &= 0x0000;
        bor |= borlevel;
        /* Enables the Option Bytes Programming operation */
        FLASH->CR |= FLASH_CR_OPTPG;
        *(__IO uint16_t*)FLASH_OB_BOR_LEVEL_ADDRESS = bor;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte Data Program Status */
    return status;
}

/**
  * @brief  Programs the FLASH User Option Byte: IWDG_RL_IV
  * @note   To correctly run this function, the FLASH_OBUnlock() function must be called before.
  * @note   Call the FLASH_OBLock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  NewState: new state of the option bytes
  *          This parameter can be: ENABLE or DISABLE
  * @param  OB_IWDG_RLR: the reload value of iwdg
  *          This parameter can be: 100-4095
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBIWDGRLRConfig(uint16_t OB_IWDG_RLR, FunctionalState NewState)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
    /* If the erase operation is completed, disable the OPTER Bit */
    FLASH->CR &= ~FLASH_CR_OPTER;

    if (NewState == ENABLE)
    {
        if (status == FLASH_COMPLETE)
        {
            FLASH->CR |= FLASH_CR_OPTPG;
            *(__IO uint16_t*)FLASH_OB_IWDG_RL_IV_ADDRESS = (uint16_t)(OB_IWDG_RLR);

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
            FLASH->CR &= ~FLASH_CR_OPTER;
        }

        if (status == FLASH_COMPLETE)
        {
            FLASH->CR |= FLASH_CR_OPTPG;
            *(__IO uint16_t*)FLASH_OB_IWDG_INI_KEY_ADDRESS = (uint16_t)(0x5b1e);

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
            FLASH->CR &= ~FLASH_CR_OPTER;
        }

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte program Status */
    return status;
}


/**
  * @brief  Programs the FLASH User Option Byte: IWDG_CLK_LP_CTL
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  NewState: new state of the option bytes
  *          This parameter can be: ENABLE or DISABLE
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBIWDGCLKConfig(FunctionalState NewState)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    /* If the erase operation is completed, disable the OPTER Bit */
    FLASH->CR &= ~FLASH_CR_OPTER;

    if (NewState == ENABLE)
    {
        if (status == FLASH_COMPLETE)
        {
            FLASH->CR |= FLASH_CR_OPTPG;
            *(__IO uint16_t*)FLASH_OB_IWDG_LP_CTL_ADDRESS = (uint16_t)(0x369C);

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
            FLASH->CR &= ~FLASH_CR_OPTER;
        }

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte program Status */
    return status;
}


/**
  * @brief  Programs the FLASH User Option Byte: NRST_SEL_KEY
  * @note   To correctly run this function, the FLASH_OBUnlock() function must be called before.
  * @note   Call the FLASH_OBLock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  NewState: new state of the option bytes
  *          This parameter can be: ENABLE or DISABLE
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBNRSTSELConfig(FunctionalState NewState)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    /* If the erase operation is completed, disable the OPTER Bit */
    FLASH->CR &= ~FLASH_CR_OPTER;

    if (NewState == ENABLE)
    {
        if (status == FLASH_COMPLETE)
        {
            FLASH->CR |= FLASH_CR_OPTPG;
            *(__IO uint16_t*)FLASH_OB_NRST_IOEN_ADDRESS = (uint16_t)(0x3546);

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
            FLASH->CR &= ~FLASH_CR_OPTER;
        }

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte program Status */
    return status;
}


/**
  * @brief  Programs the FLASH User Option Byte: DBG_CLK_KEY
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  NewState: new state of the option bytes
  *          This parameter can be: ENABLE or DISABLE
  * @retval FLASH Status: The returned value can be:
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBDBGCLKConfig(FunctionalState NewState)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    /* If the erase operation is completed, disable the OPTER Bit */
    FLASH->CR &= ~FLASH_CR_OPTER;

    if (NewState == ENABLE)
    {
        if (status == FLASH_COMPLETE)
        {
            FLASH->CR |= FLASH_CR_OPTPG;
            *(__IO uint16_t*)FLASH_OB_DBG_CTL_KEY1_ADDRESS = (uint16_t)(0x1234bcde);
			*(__IO uint16_t*)FLASH_OB_DBG_CTL_KEY2_ADDRESS = (uint16_t)(0x1234);
            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);
            FLASH->CR &= ~FLASH_CR_OPTER;
        }

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte program Status */
    return status;
}


/**
  * @brief  Programs a word at a specified Option Byte Data address.
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  Address: specifies the address to be programmed.
  *          This parameter can be 0x1FFFF800 to 0x1FFFF840.
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBProgram(uint32_t Address, uint16_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        /* Enables the Option Bytes Programming operation */
        FLASH->CR |= FLASH_CR_OPTPG;
        *(__IO uint16_t*)Address = Data;

        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte Data Program Status */
    return status;
}

/**
  * @brief  Programs a word at a specified Option Byte Data address.
  * @note   To correctly run this function, the FLASH_OB_Unlock() function must be called before.
  * @note   Call the FLASH_OB_Lock() to disable the flash control register access and the option
  *         bytes (recommended to protect the FLASH memory against possible unwanted operation)
  * @param  Data: specifies the data to be programmed.
  * @retval FLASH Status: The returned value can be: FLASH_ERROR_PG,
  *         FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_OBProgramData(uint32_t Data)
{
    FLASH_Status status = FLASH_COMPLETE;
    status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

    if (status == FLASH_COMPLETE)
    {
        /* Enables the Option Bytes Programming operation */
        FLASH->CR |= FLASH_CR_OPTPG;
        *(__IO uint16_t*)0x1FFFF804 = (uint16_t)Data;
		*(__IO uint16_t*)0x1FFFF806 = (uint16_t)(Data>>16);
        /* Wait for last operation to be completed */
        status = FLASH_WaitForLastOperation(FLASH_ER_PRG_TIMEOUT);

        if (status != FLASH_TIMEOUT)
        {
            /* If the program operation is completed, disable the OPTPG Bit */
            FLASH->CR &= ~FLASH_CR_OPTPG;
        }
    }

    /* Return the Option Byte Data Program Status */
    return status;
}

/**
  * @brief  Returns the FLASH Write Protection Option Bytes value.
  * @note   None
  * @retval The FLASH Write Protection Option Bytes value
  */
uint32_t FLASH_OBGetWRP(void)
{
    /* Return the FLASH write protection Register value */
    return (uint32_t)(FLASH->WRPR);
}

/**
  * @brief  Checks whether the FLASH Read out Protection Status is set or not.
  * @note   None
  * @retval FLASH ReadOut Protection Status(SET or RESET)
  */
FlagStatus FLASH_OBGetRDP(void)
{
    FlagStatus readstatus = RESET;

    if ((uint8_t)(FLASH->OBR & (FLASH_OBR_RDPRT_0 | FLASH_OBR_RDPRT_1)) != RESET)
    {
        readstatus = SET;
    }
    else
    {
        readstatus = RESET;
    }

    return readstatus;
}

/**
  * @}
  */

/** @defgroup FLASH_Group4 Interrupts and flags management functions
  * @note   None
  * @brief   Interrupts and flags management functions
  *
@verbatim
 ===============================================================================
             ##### Interrupts and flags management functions #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @note   None
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or
  *         disabled.
  *          This parameter can be any combination of the following values:
  *             @arg FLASH_IT_EOP: FLASH end of programming Interrupt
  *             @arg FLASH_IT_ERR: FLASH Error Interrupt
  * @param  NewState: new state of interrupt sources.
  *         This parameter can be:  ENABLE or DISABLE.
  * @retval None
  */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FLASH_IT(FLASH_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        /* Enable the interrupt sources */
        FLASH->CR |= FLASH_IT;
    }
    else
    {
        /* Disable the interrupt sources */
        FLASH->CR &= ~(uint32_t)FLASH_IT;
    }
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @note   None
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *          This parameter can be one of the following values:
  *             @arg FLASH_FLAG_BSY: FLASH write/erase operations in progress flag
  *             @arg FLASH_FLAG_PGERR: FLASH Programming error flag flag
  *             @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag
  *             @arg FLASH_FLAG_EOP: FLASH End of Programming flag
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));

    if ((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the new state of FLASH_FLAG (SET or RESET) */
    return bitstatus;
}

/**
  * @brief  Clears the FLASH's pending flags.
  * @note   None
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *          This parameter can be any combination of the following values:
  *             @arg FLASH_FLAG_PGERR: FLASH Programming error flag flag
  *             @arg FLASH_FLAG_WRPERR: FLASH Write protected error flag
  *             @arg FLASH_FLAG_EOP: FLASH End of Programming flag
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FLASH_FLAG)
{
    /* Check the parameters */
    assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG));

    // unlock the flash to reset the falsh_cr_lock
    if ((FLASH->CR & FLASH_CR_LOCK)  != RESET)
    {
        /* Clear the flags */
        FLASH->SR = FLASH_FLAG;
    }
    else
    {
        /* Clear the flags */
        FLASH->SR = FLASH_FLAG;
    }

}

/**
  * @brief  Returns the FLASH Status.
  * @note   None
  * @retval FLASH Status: The returned value can be:
  *         FLASH_BUSY, FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP or FLASH_COMPLETE.
  */
FLASH_Status FLASH_GetStatus(void)
{
    FLASH_Status FLASHstatus = FLASH_COMPLETE;

    if ((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY)
    {
        FLASHstatus = FLASH_BUSY;
    }
    else
    {
        if ((FLASH->SR & (uint32_t)FLASH_FLAG_WRPERR) != (uint32_t)0x00)
        {
            FLASHstatus = FLASH_ERROR_WRP;
        }
        else
        {
            if ((FLASH->SR & (uint32_t)(FLASH_SR_PGERR)) != (uint32_t)0x00)
            {
                FLASHstatus = FLASH_ERROR_PROGRAM;
            }
            else
            {
                FLASHstatus = FLASH_COMPLETE;
            }
        }
    }

    /* Return the FLASH Status */
    return FLASHstatus;
}


/**
  * @brief  Waits for a FLASH operation to complete or a TIMEOUT to occur.
  * @note   None
  * @param  Timeout: FLASH programming Timeout
  * @retval FLASH Status: The returned value can be: FLASH_BUSY,
  *         FLASH_ERROR_PROGRAM, FLASH_ERROR_WRP, FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{
    FLASH_Status status = FLASH_COMPLETE;

    /* Check for the FLASH Status */
    status = FLASH_GetStatus();

    /* Wait for a FLASH operation to complete or a TIMEOUT to occur */
    while ((status == FLASH_BUSY) && (Timeout != 0x00))
    {
        status = FLASH_GetStatus();
        Timeout--;
    }

    if (Timeout == 0x00 )
    {
        status = FLASH_TIMEOUT;
    }

    /* Return the operation status */
    return status;
}
/**
  * @brief Return the unique device identifier (UID based on 96 bits)
  * @param UID: pointer to 3 words array.
  * @retval void
  */
void Sys_GetDevice96BitUID(uint32_t *UID)
{
    UID[0] = *((uint32_t *)UID_BASE);
    UID[1] = *((uint32_t *)(UID_BASE + 4U));
    UID[2] = *((uint32_t *)(UID_BASE + 8U));
}

/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
