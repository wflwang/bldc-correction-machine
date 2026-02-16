/********************************************************************
* @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd 
* @Filename : hk32m07x_pga.h
* @brief    : API file of pga module 
* @Author   : AE Team  
  * @version V1.0.0/2024-12-16
  * @log  V1.0.0/2024-12-16 Initial release
 ********************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __hk32m07x_PGA_H
#define __hk32m07x_PGA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hk32m07x.h"

/** @addtogroup PGA
  * @{
  */
	
/** @defgroup PGA_Exported_Types PGA_Exported_Types
  * @{
  */
#define PGA1    1
#define PGA2    2
/**
  * @brief PGA1 Init structure definition
  */
typedef struct
{
    uint32_t PGA1_NPLockMode;           /*PGA1负端锁定功能选择*/         

    uint32_t PGA1_PBInputSelect;         /*PGA1_PBx输入信号源选择*/  

    uint32_t PGA1_PAInputSelect;         /*PGA1_PAx输入信号源选择*/     

    uint32_t PGA1_CHInputSelect;       /*PGA1输入信号源（PGA1_PAx/NA,PGA1_PBx/NB）选择*/
            
    uint32_t PGA1_NInputSelect;         /*PGA1负端输入信号源（外部PIN,内部AVSS）选择*/ 

    uint32_t PSAMSelect;            /*PGA1输入通道信进行A/B轮巡时，自动或者手动切换A组/B组PIN*/ 
    
    uint32_t PGA1_GAIN;             /*PGA1_GAIN,PGA1增益放大倍数*/ 
} PGA1_InitTypeDef;
 
/**
  * @brief PGA2 Init structure definition
  */
typedef struct
{   
    uint32_t PGA2_PInputSelect;       /*PGA2正端输入信号源（PGA2_P1,PGA2_P2,PGA1_OUTA(B),PGA1_NA,PGA1_NB）选择*/ 
           
    uint32_t PGA2_NInputSelect;         /*PGA2负端输入信号源（PGA2_N1,PGA2_N2,内部AVSS）选择*/ 
    
    uint32_t PGA2_GAIN;             /*PGA2_GAIN,PGA2增益放大倍数*/ 
} PGA2_InitTypeDef;


  
/** @defgroup PGA_Define PGA_Define
  * @{
  */

#define PGA_CAL_TIMOUT					16384

#define VrefDiv1                        ((uint32_t)0x00000000<<PGA_PBVSR_PBIAS_Pos)                                    
#define VrefDiv2                        ((uint32_t)0x00000001<<PGA_PBVSR_PBIAS_Pos)                       
#define VrefDiv3                        ((uint32_t)0x00000002<<PGA_PBVSR_PBIAS_Pos)                     
#define VrefDiv4                       	((uint32_t)0x00000003<<PGA_PBVSR_PBIAS_Pos)                      

#define PGA_PCSR1_PGA_Pos                 ((uint32_t)0) 
/*********************************PGA1 PGA Gain***************************************************************/
#define PGA1_PGA_GAIN_1                        ((uint32_t)0x00000000<<PGA_PCSR1_PGA_Pos)                                    
#define PGA1_PGA_GAIN_4                        ((uint32_t)0x00000002<<PGA_PCSR1_PGA_Pos)                       
#define PGA1_PGA_GAIN_8                        ((uint32_t)0x00000003<<PGA_PCSR1_PGA_Pos)                     
#define PGA1_PGA_GAIN_12                       ((uint32_t)0x00000005<<PGA_PCSR1_PGA_Pos)                      
#define PGA1_PGA_GAIN_20                       ((uint32_t)0x00000007<<PGA_PCSR1_PGA_Pos) 
#define PGA1_PGA_GAIN_40                       ((uint32_t)0x00000006<<PGA_PCSR1_PGA_Pos)
/**************************************************************************************************************/

#define PGA_PCSR2_PGA_Pos                 ((uint32_t)0) 
/*********************************PGA2 PGA Gain***************************************************************/
#define PGA2_PGA_GAIN_1                        ((uint32_t)0x00000000<<PGA_PCSR2_PGA_Pos)                                    
#define PGA2_PGA_GAIN_4                        ((uint32_t)0x00000002<<PGA_PCSR2_PGA_Pos)                       
#define PGA2_PGA_GAIN_8                        ((uint32_t)0x00000003<<PGA_PCSR2_PGA_Pos)                     
#define PGA2_PGA_GAIN_12                       ((uint32_t)0x00000005<<PGA_PCSR2_PGA_Pos)                      
#define PGA2_PGA_GAIN_20                       ((uint32_t)0x00000007<<PGA_PCSR2_PGA_Pos)
#define PGA2_PGA_GAIN_40                       ((uint32_t)0x00000006<<PGA_PCSR2_PGA_Pos)
/**************************************************************************************************************/

#define PGA_PCSR1_PNPL_Pos                 ((uint32_t)12) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA1_NPLock_N0      ((uint32_t)0x00000000<<PGA_PCSR1_PNPL_Pos)  
#define PGA1_NPLock_NA      ((uint32_t)0x00000001<<PGA_PCSR1_PNPL_Pos)    
#define PGA1_NPLock_NB      ((uint32_t)0x00000002<<PGA_PCSR1_PNPL_Pos)    
/**************************************************************************************************************/

#define PGA_PCSR1_PBIS_Pos                 ((uint32_t)10) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA1_PB1      ((uint32_t)0x00000000<<PGA_PCSR1_PBIS_Pos)  
#define PGA1_PB2      ((uint32_t)0x00000001<<PGA_PCSR1_PBIS_Pos)      
/**************************************************************************************************************/

#define PGA_PCSR1_PAIS_Pos                 ((uint32_t)9) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA1_PA1      ((uint32_t)0x00000000<<PGA_PCSR1_PAIS_Pos)  
#define PGA1_PA2      ((uint32_t)0x00000001<<PGA_PCSR1_PAIS_Pos)      
/**************************************************************************************************************/

#define PGA_PCSR1_PCHS_Pos                 ((uint32_t)8) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA1_InputSelectPANA      ((uint32_t)0x00000000<<PGA_PCSR1_PCHS_Pos)  
#define PGA1_InputSelectPBNB      ((uint32_t)0x00000001<<PGA_PCSR1_PCHS_Pos)      
/**************************************************************************************************************/

#define PGA_PCSR1_PNIS_Pos                 ((uint32_t)6) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA1_NInputSelectIO         ((uint32_t)0x00000000<<PGA_PCSR1_PNIS_Pos)  
#define PGA1_NInputSelectAVSS       ((uint32_t)0x00000001<<PGA_PCSR1_PNIS_Pos)      
/**************************************************************************************************************/

#define PGA_PCSR1_PSAM_Pos                 ((uint32_t)3) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA1_PSAMSelectAuto         ((uint32_t)0x00000000<<PGA_PCSR1_PSAM_Pos)  
#define PGA1_PSAMSelectHand         ((uint32_t)0x00000001<<PGA_PCSR1_PSAM_Pos)      
/**************************************************************************************************************/

#define PGA_PCSR2_PPIS_Pos                 ((uint32_t)8) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA2_PInputSelect_P1            ((uint32_t)0x00000000<<PGA_PCSR2_PPIS_Pos)  
#define PGA2_PInputSelect_P2            ((uint32_t)0x00000001<<PGA_PCSR2_PPIS_Pos)    
#define PGA2_PInputSelect_PGA1OUT       ((uint32_t)0x00000002<<PGA_PCSR2_PPIS_Pos)   
#define PGA2_PInputSelect_PGA1NA        ((uint32_t)0x00000003<<PGA_PCSR2_PPIS_Pos)    
#define PGA2_PInputSelect_PGA1NB        ((uint32_t)0x00000004<<PGA_PCSR2_PPIS_Pos)  
/**************************************************************************************************************/

#define PGA_PCSR2_PNIS_Pos                 ((uint32_t)6) 
/*********************************PGA Non-inverting Input****************************************************/
#define PGA2_NInputSelectN1         ((uint32_t)0x00000000<<PGA_PCSR2_PNIS_Pos)  
#define PGA2_NInputSelectN2         ((uint32_t)0x00000001<<PGA_PCSR2_PNIS_Pos)  
#define PGA2_NInputSelectAVSS       ((uint32_t)0x00000002<<PGA_PCSR2_PNIS_Pos)      
/**************************************************************************************************************/


/**
  * @}
  */
  
/**
  * @}
  */	
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/** @defgroup PGA_Exported_Functions PGA_Exported_Functions
  * @{
  */
void PGA_DeInit(void);

void  PGA_Init(PGA1_InitTypeDef *PGA1_InitStruct ,PGA2_InitTypeDef *PGA2_InitStruct);

void PGA_SetBias( FunctionalState PGA1_NewState, FunctionalState PGA2_NewState, uint8_t VrefDiv);

void PGA_SetPGAGain(uint32_t PGAx, uint32_t PGAGain);

uint32_t PGA_GetPGAGain(uint32_t PGAx);

void PGA1_NPLock_Set(uint32_t NPLockSelect);

uint32_t PGA_GetNPLock(void);

void PGA1_PBSelect(uint32_t PBSelect);

void PGA1_PASelect(uint32_t PASelect);

void PGA1_PCHSelect(uint32_t PCHSelect);

void PGA1_NSelect(uint32_t NSelect);

void PGA1_PSAMSelect(uint32_t PSAMSelect);

void PGA2_PSelect(uint32_t PSelect);

void PGA2_NSelect(uint32_t PSelect);

void PGA_Lock(void);

void PGA_UNLock(void);

void PGA_Enable(uint32_t PGAx);

void PGA_Disable(uint32_t PGAx);

void PGA_VIM_TRIM(uint32_t PGAx);

void PGA_VIM_USER(uint32_t PGAx);

void PGA_USER_Default(void);

uint16_t PGA_SW_Calibration(uint32_t PGAx, uint32_t PGAGain);
/**
  * @}
  */

/**
  * @}
  */
 
#ifdef __cplusplus
}
#endif

#endif /* __hk32m07x_PGA_H */
 
/***************************** (C) COPYRIGHT HKMicroChip  *******************************/
