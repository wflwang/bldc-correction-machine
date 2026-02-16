/********************************************************************
* @Copyright: Shenzhen Hangshun Chip Technology R&D Co., Ltd 
* @Filename : hk32m07x_PGA.c
* @brief    : API file of PGA module 
* @Author   : AE Team  
* @Version  : V1.0.0/2024-012-16
  * @log  V1.0.0/2024-12-16 Initial release
 ********************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "hk32m07x_pga.h"
#include "hk32m07x_rcc.h"


/** @addtogroup hk32m07x_StdPeriph_Driver
  * @{
  */

/** @defgroup PGA PGA 
  * @brief PGA modules
  * @{
  */

/** @defgroup PGA_Private_Functions PGA_Private_Functions
  * @{
  */

/**
  * @brief  Deinitializes the PGA peripheral registers to their default reset values.
  */
void PGA_DeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_PGA, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_PGA, DISABLE);
}

/**
  * @brief  Initialize some features of PGA instance.
  * @param  PGA1_InitStruct: Pointer to PGA1_InitTypeDef structure.
  * @param  PGA2_InitStruct: Pointer to PGA2_InitTypeDef structure.
  */
void  PGA_Init(PGA1_InitTypeDef *PGA1_InitStruct ,PGA2_InitTypeDef *PGA2_InitStruct)
{
    PGA_UNLock();
    PGA->PCSR1 &=0x0000;
    PGA->PCSR1 |=(PGA1_InitStruct->PGA1_CHInputSelect |
                 PGA1_InitStruct->PGA1_NPLockMode |
	             PGA1_InitStruct->PGA1_NInputSelect |
                 PGA1_InitStruct->PGA1_PBInputSelect |
                 PGA1_InitStruct->PGA1_PAInputSelect |
                 PGA1_InitStruct->PSAMSelect |
                 PGA1_InitStruct->PGA1_GAIN 
                    );
	PGA->PCSR2 &=0x0000;
    PGA->PCSR2 |=(PGA2_InitStruct->PGA2_NInputSelect |
                 PGA2_InitStruct->PGA2_PInputSelect |
                 PGA2_InitStruct->PGA2_GAIN 
                    );
}

/**
  * @brief  Set PGA bias voltage.
  * @param  PGA1_NewState: ENABLE or DISABLE.
  * @param  PGA2_NewState: ENABLE or DISABLE.
  * @param  Bias: VrefDiv1=Vref/2,VrefDiv2=Vref/4,VrefDiv3=Vref/8,VrefDiv4=Vref/16.
  */
void PGA_SetBias( FunctionalState PGA1_NewState, FunctionalState PGA2_NewState, uint8_t Bias)
{
    /* Set PGA1&2 bias voltage */
	PGA->PBVSR = 0x0000;
    PGA->PBVSR |= Bias;	
	
    /* Enable PGA2 bias voltage */
    if(PGA2_NewState != DISABLE)
    {
        PGA->PBVSR |= 0x0020;
    }else
    {
        PGA->PBVSR &=0xFFDF;
    }	
    /* Enable PGA1 bias voltage */
    if(PGA1_NewState != DISABLE)
    {
        PGA->PBVSR |= 0x0010;
    }else
    {
        PGA->PBVSR &=0xFFEF;
    }
}

/**
  * @brief  Set PGA PGA gain.
  * @param  PGAx: PGA1,PGA2.
  * @param  PGAGain:
  *         	@arg PGA1_PGA_GAIN_1/PGA2_PGA_GAIN_1.
  *         	@arg PGA1_PGA_GAIN_4/PGA2_PGA_GAIN_4.
  *         	@arg PGA1_PGA_GAIN_8/PGA2_PGA_GAIN_8.
  *         	@arg PGA1_PGA_GAIN_12/PGA2_PGA_GAIN_12.
  *         	@arg PGA1_PGA_GAIN_20/PGA2_PGA_GAIN_20. 
  *         	@arg PGA1_PGA_GAIN_40/PGA2_PGA_GAIN_40. 
  */
void PGA_SetPGAGain(uint32_t PGAx, uint32_t PGAGain)
{
    if(PGAx == PGA1)
    {
        PGA->PCSR1 &= ~(PGA_PCSR1_PGA);
        PGA->PCSR1 |= PGAGain;
    }
    if(PGAx == PGA2)
    {
        PGA->PCSR2 &= ~(PGA_PCSR2_PGA);
        PGA->PCSR2 |= PGAGain;
    }
}
/**
  * @brief  Get PGA gain.
  * @param  PGAx: PGA1,PGA2.
  * @retval PGA gain.
  *         0:GAIN=1.
  *         2:GAIN=4.
  *         3:GAIN=8.
  *         5:GAIN=12.
  *         7:GAIN=20.
  *         6:GAIN=40.
  */
uint32_t PGA_GetPGAGain(uint32_t PGAx)
{
    if(PGAx == PGA1)
    {
        return (PGA->PCSR1 &= PGA_PCSR1_PGA);        
    }
    else if(PGAx == PGA2)
    {
        return (PGA->PCSR2 &= PGA_PCSR2_PGA);
    }else 
        return 0;   
}
/**
  * @brief  PGA1_NPLock Set.
  * @param  NPLockSelect: Negative input Lock set,PGA1_NPLock_N0 or PGA1_NPLock_NA or PGA1_NPLock_NB.
  */
void PGA1_NPLock_Set(uint32_t NPLockSelect)
{
    PGA->PCSR1 &= ~(PGA_PCSR1_PNPL);
    PGA->PCSR1 |=NPLockSelect;
}
/**
  * @brief  Get PGA1 NPLock.
  * @retval NPLockSelect.
  *         0:PGA1_NPLock_N0.
  *         1:PGA1_NPLock_NA.
  *         2:PGA1_NPLock_NB.
  *         3:PGA1_NPLock_N0.
  */
uint32_t PGA_GetNPLock(void)
{
    return (PGA->PCSR1 &= PGA_PCSR1_PNPL)>>12;
}
/**
  * @brief  PGA1_PB Select.
  * @param  PBSelect: PGA1_PB input Select,PGA1_PB1 or PGA1_PB2.
  */
void PGA1_PBSelect(uint32_t PBSelect)
{
    PGA->PCSR1 &= ~(PGA_PCSR1_PBIS);
    PGA->PCSR1 |=PBSelect;
}

/**
  * @brief  PGA1_PA Select.
  * @param  PASelect: PGA1_PA input Select,PGA1_PA1 or PGA1_PA2.
  */
void PGA1_PASelect(uint32_t PASelect)
{
    PGA->PCSR1 &= ~(PGA_PCSR1_PAIS);
    PGA->PCSR1 |=PASelect;
}
/**
  * @brief  PGA1_PCH Select.
  * @param  PCHSelect: PGA1_PCH input Select,PGA1_InputSelectPANA or PGA1_InputSelectPBNB.
  */
void PGA1_PCHSelect(uint32_t PCHSelect)
{
    PGA->PCSR1 &= ~(PGA_PCSR1_PCHS);
    PGA->PCSR1 |=PCHSelect;
}
/**
  * @brief  PGA1_N Select.
  * @param  NSelect: PGA1_N input Select,PGA1_NInputSelectIO or PGA1_NInputSelectAVSS.
  */
void PGA1_NSelect(uint32_t NSelect)
{
    PGA->PCSR1 &= ~(PGA_PCSR1_PNIS);
    PGA->PCSR1 |=NSelect;
}
/**
  * @brief  PGA1_PSAM Select.
  * @param  PSAMSelect: PGA1_PSAM Select,PGA1_PSAMSelectAuto or PGA1_PSAMSelectHand.
  */
void PGA1_PSAMSelect(uint32_t PSAMSelect)
{
    PGA->PCSR1 &= ~(PGA_PCSR1_PSAM);
    PGA->PCSR1 |=PSAMSelect;
}
 
/**
  * @brief  PGA2_PSelect.
  * @param  PSelect: PGA2_P input Select,PGA2_PInputSelect_P1 or PGA2_PInputSelect_P2 or PGA2_PInputSelect_PGA1OUT or PGA2_PInputSelect_PGA1NA or PGA2_PInputSelect_PGA1NB.
  */
void PGA2_PSelect(uint32_t PSelect)
{
    PGA->PCSR2 &= ~(PGA_PCSR2_PPIS);
    PGA->PCSR2 |=PSelect;
}
/**
  * @brief  PGA2_N Select.
  * @param  NSelect: PGA2_N input Select,PGA2_NInputSelectN1 or PGA2_NInputSelectN2 or PGA2_NInputSelectAVSS.
  */
void PGA2_NSelect(uint32_t NSelect)
{
    PGA->PCSR1 &= ~(PGA_PCSR2_PNIS);
    PGA->PCSR1 |=NSelect;
}
/**
  * @brief  Lock PGA,The PGA register cannot be written.
  */
void PGA_Lock(void)
{
    PGA->PRWPT = 0x0000;
}
/**
  * @brief  UNLock PGA,Disable PGA write protection.
  */
void PGA_UNLock(void)
{
    PGA->PRWPT = 0xA5A5;
}

/**
  * @brief  VIM source selection,ENABLE PGA trimming mode.
  * @param  PGAx: PGA1,PGA2.
  */
void PGA_VIM_TRIM(uint32_t PGAx)
{
	if(PGAx == PGA1)
    {
		PGA->PCALCSR1 = (uint32_t)(PGA->PCALCSR1 & 0xCFFF);
		PGA->PCALCSR1 |= (uint32_t)0x2 << PGA_PCALCSR2_VIM_SEL_Pos;
	}
	else if(PGAx == PGA2)
	{
		PGA->PCALCSR2 = (uint32_t)(PGA->PCALCSR2 & 0xCFFF);
		PGA->PCALCSR2 |= (uint32_t)0x2 << PGA_PCALCSR2_VIM_SEL_Pos;
	}
}
/**
  * @brief  VIM source selection,ENABLE PGA User mode.
  * @param  PGAx: PGA1,PGA2.
  */
void PGA_VIM_USER(uint32_t PGAx)
{
	if(PGAx == PGA1)
    {
		PGA->PCALCSR1 = (uint32_t)(PGA->PCALCSR1 & 0xCFFF);
		PGA->PCALCSR1 |= (uint32_t)0x1 << PGA_PCALCSR2_VIM_SEL_Pos;
	}
	else if(PGAx == PGA2)
	{
		PGA->PCALCSR2 = (uint32_t)(PGA->PCALCSR2 & 0xCFFF);
		PGA->PCALCSR2 |= (uint32_t)0x1 << PGA_PCALCSR2_VIM_SEL_Pos;
	}
}

/**
  * @brief  PGA User mode Default set.
  */
void PGA_USER_Default(void)
{
	PGA->PCALCSR1 = (uint32_t)(PGA->PCALCSR1 & 0xCFFF);
	PGA->PCALCSR1 |= (uint32_t)0x1 << PGA_PCALCSR2_VIM_SEL_Pos|PGA_PCALCSR1_TEIMVOSP_4;
	
	PGA->PCALCSR2 = (uint32_t)(PGA->PCALCSR2 & 0xCFFF);
	PGA->PCALCSR2 |= (uint32_t)0x1 << PGA_PCALCSR2_VIM_SEL_Pos|PGA_PCALCSR2_TEIMVOSP_4;	
}

/**
  * @brief  PGA Software Calibration.
  * @param  PGAx: PGA1,PGA2.
  * @param  PGAGain:
  *         	@arg PGA1_PGA_GAIN_1/PGA2_PGA_GAIN_1.
  *         	@arg PGA1_PGA_GAIN_4/PGA2_PGA_GAIN_4.
  *         	@arg PGA1_PGA_GAIN_8/PGA2_PGA_GAIN_8.
  *         	@arg PGA1_PGA_GAIN_12/PGA2_PGA_GAIN_12.
  *         	@arg PGA1_PGA_GAIN_20/PGA2_PGA_GAIN_20. 
  *         	@arg PGA1_PGA_GAIN_40/PGA2_PGA_GAIN_40. 
  * @retval PMOS Offset (Indicates the calibration value).
  */
uint16_t PGA_SW_Calibration(uint32_t PGAx, uint32_t PGAGain)
{
	uint16_t DelayCNT = 0;
	uint16_t Cal_Value = 0;	
	uint8_t PmosADJ_CNT = 0;
	uint16_t CalSR_Value = 0;
	PGA_UNLock();
	PGA_Enable(PGA1);
	PGA_Enable(PGA2);	
	PGA_VIM_TRIM(PGA1);
	PGA_VIM_TRIM(PGA2);
	PGA->PBVSR =0x0000;
    if(PGAx == PGA1)
    {
		for(DelayCNT=0;DelayCNT<1000;DelayCNT++) //Wait for stability
		{
			__NOP();
		}
		PGA_SetPGAGain(PGA1,PGAGain);
		if(PGAGain ==PGA1_PGA_GAIN_1)//GAIN =1
		{
			PGA->PBVSR |= VrefDiv1|0x0010;
			
		}else//GAIN >1
		{
			PGA->PBVSR |= VrefDiv4|0x0010;			
		} 
		PGA->PCALCSR1 &= 0xFFC0;
		PGA->PCALCSR1 |= 0x0020;//Pmos initial
		for(PmosADJ_CNT = 4; PmosADJ_CNT>=1; )
		{
			
			for(DelayCNT=0;DelayCNT<2000;DelayCNT++) //Wait for stability
			{
				__NOP();
			}
			if(PGA->PCALCSR1 & 0x8000)
			{
				CalSR_Value = PGA->PCALCSR1;
				PGA->PCALCSR1 = (CalSR_Value-(1<<(PmosADJ_CNT+1))+(1<<PmosADJ_CNT));				
			}
			else
			{
				CalSR_Value = PGA->PCALCSR1;
				PGA->PCALCSR1 = (CalSR_Value+(1<<PmosADJ_CNT));
			}
			PmosADJ_CNT--;
		}
		Cal_Value =((PGA->PCALCSR1) & 0x003E)>>1;
        return Cal_Value;        
    }
    else if(PGAx == PGA2)
    {
		PGA_Enable(PGA2);
		for(DelayCNT=0;DelayCNT<1000;DelayCNT++) //Wait for stability
		{
			__NOP();
		}
		PGA_SetPGAGain(PGA2,PGAGain);
		if(PGAGain ==PGA2_PGA_GAIN_1)//GAIN =1
		{
			PGA->PBVSR |= VrefDiv1|0x0020;
			
		}else//GAIN >1
		{
			PGA->PBVSR |= VrefDiv4|0x0020;			
		} 
		PGA->PCALCSR2 &= 0xFFC0;
		PGA->PCALCSR2 |= 0x0020;//Pmos initial
		for(PmosADJ_CNT = 4; PmosADJ_CNT>=1; )
		{
			for(DelayCNT=0;DelayCNT<2000;DelayCNT++) //Wait for stability
			{
				__NOP();
			}
			if(PGA->PCALCSR2 & 0x8000)
			{
				CalSR_Value = PGA->PCALCSR2;
				PGA->PCALCSR2 = (CalSR_Value-(1<<(PmosADJ_CNT+1))+(1<<PmosADJ_CNT));
			}
			else
			{
				CalSR_Value = PGA->PCALCSR2;
				PGA->PCALCSR2 = (CalSR_Value+(1<<PmosADJ_CNT));				
			}
			PmosADJ_CNT--;
		}
		Cal_Value =((PGA->PCALCSR2) & 0x003E)>>1;
        return Cal_Value;        
    }else 
        return 0;   
}

/**
  * @brief  Enable PGA.
  * @param  PGAx: PGA1,PGA2.
  */
void PGA_Enable(uint32_t PGAx)
{
    if(PGAx == PGA1)
    {
        PGA->PCSR1 |= PGA_PCSR1_PEN;        
    }
    if(PGAx == PGA2)
    {
        PGA->PCSR2 |= PGA_PCSR2_PEN; 
    }
}

/**
  * @brief  Disable PGA.
  * @param  PGAx: PGA1,PGA2.
  */
void PGA_Disable(uint32_t PGAx)
{
    if(PGAx == PGA1)
    {
        PGA->PCSR1 &= (uint32_t) (~PGA_PCSR1_PEN);
    }
    if(PGAx == PGA2)
    {
         PGA->PCSR2 &= (uint32_t) (~PGA_PCSR2_PEN);
    }
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
 
