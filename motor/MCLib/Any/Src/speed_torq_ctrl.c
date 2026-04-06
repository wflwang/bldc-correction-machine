/*
  ******************************************************************************
  * @file    speed_torq_ctrl.c
	* @version V1.0.1
	* @date    2021-07-22
 ===========================================================================
                    ##### How to control speed and torque #####
 ===========================================================================
		 [..]
     (#) This file provides firmware functions that implement the following features
         of the Speed & Torque Control component of the Motor Control SDK.
    @endverbatim
  ****************************************************************************** 
  */

/* Includes ------------------------------------------------------------------*/
#include "speed_torq_ctrl.h"
#include "speed_pos_fdbk.h"

#include "mc_type.h"
#include "mc_tasks.h"
#include "hw_correct.h"

#define CHECK_BOUNDARY

static int speedcount=0;  //出发目标速度计数
static int speedNum=0;  //记低速出发次数
static int ErrorStopCount=0;  //累计大于误差的次数
//static int32_t MinErrS = cMinErrH;
static int LhError=0;
static int16_t id_weak_field_filtered = 0;  //上次弱磁限制
static int16_t smoothed_brake_limit = CurrentInt16(DefaultMinBrakeCurrent);; //上次最大刹车限制
static int16_t lastTorq = 0;  //上次的输出转矩

/** @addtogroup MCSDK
  * @{
  */

/** @defgroup initialization and reset module  
  * @brief Speed & torque control related variables initialization
 
@verbatim
 ===============================================================================
      ##### Initialize and reset SpeednTorqCtrl related component #####
 ===============================================================================
@endverbatim
  * @{
  */
	
/**
  * @brief  Initializes all the object variables, usually it has to be called
  *         once right after object creation.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @param  oPI the PI object used as controller for the speed regulation.
  *         It can be equal to MC_NULL if the STC is initialized in torque mode
  *         and it will never be configured in speed mode.
  * @param  oSPD the speed sensor used to perform the speed regulation.
  *         It can be equal to MC_NULL if the STC is used only in torque
  *         mode.
  * @retval none.
  */
void STC_Init( SpeednTorqCtrl_Handle_t * pHandle, PID_Handle_t * pPI, SpeednPosFdbk_Handle_t * SPD_Handle )
{
  /* Handle of a PID speed control loop component */
  pHandle->PISpeed = pPI;
	/* SpeednPosFdbk  handle definition component */
  pHandle->SPD = SPD_Handle;
  pHandle->Mode = pHandle->ModeDefault;
  pHandle->SpeedRef01HzExt = ( int32_t )pHandle->MecSpeedRef01HzDefault * 65536;
  pHandle->TorqueRef = ( int32_t )pHandle->TorqueRefDefault * 65536;
  pHandle->TargetFinal = 0;
  pHandle->RampRemainingStep = 0u;
  pHandle->IncDecAmount = 0;
}

/**
  * @brief  It should be called before each motor restart. If STC is set in
            speed mode, this method resets the integral term of speed regulator.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval none.
  */
void STC_Clear( SpeednTorqCtrl_Handle_t * pHandle )
{
  if ( pHandle->Mode == STC_SPEED_MODE )
  {
		/* Update value of IntegralTerm */
    PID_SetIntegralTerm( pHandle->PISpeed, 0 );
  }
}


/** @defgroup  Speed Control related module
  * @brief Speed Control component of the Motor Control SDK
 
@verbatim
 ===============================================================================
                    ##### Speed control functions #####
 ===============================================================================
@endverbatim
  * @{
  */

/**
  * @brief It sets in real time the speed sensor utilized by the STC.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @param SPD_Handle Speed sensor component to be set.
  * @retval none
  */
void STC_SetSpeedSensor( SpeednTorqCtrl_Handle_t * pHandle, SpeednPosFdbk_Handle_t * SPD_Handle )
{
  pHandle->SPD = SPD_Handle;
}

/**
  * @brief It returns the speed sensor utilized by the FOC.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval SpeednPosFdbk_Handle_t speed sensor utilized by the FOC.
  */
SpeednPosFdbk_Handle_t * STC_GetSpeedSensor( SpeednTorqCtrl_Handle_t * pHandle )
{
  return ( pHandle->SPD );
}

/**
  * @brief  Get the Default mechanical rotor speed reference expressed in tenths
  *         of HZ.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval int16_t It returns the Default mechanical rotor speed. reference
  *         expressed in tenths of HZ.
  */
int16_t STC_GetMecSpeedRef01HzDefault( SpeednTorqCtrl_Handle_t * pHandle )
{
  return pHandle->MecSpeedRef01HzDefault;
}

/**
  * @brief  Get the current mechanical rotor speed reference expressed in tenths
  *         of HZ.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval int16_t current mechanical rotor speed reference expressed in tenths
  *         of HZ.
  */
int16_t STC_GetMecSpeedRef01Hz( SpeednTorqCtrl_Handle_t * pHandle )
{
  return ( ( int16_t )( pHandle->SpeedRef01HzExt / 65536 ) );
}

/**
  * @brief  Get the Application maximum positive value of rotor speed. It's
            expressed in tenth of mechanical Hertz.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval uint16_t It returns the application maximum positive value of rotor
            speed expressed in tenth of mechanical Hertz.
  */
uint16_t STC_GetMaxAppPositiveMecSpeed01Hz( SpeednTorqCtrl_Handle_t * pHandle )
{
  return pHandle->MaxAppPositiveMecSpeed01Hz;
}

/**
  * @brief  Stop the execution of speed ramp.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval bool It returns true if the command is executed, false otherwise.
  */
bool STC_StopSpeedRamp( SpeednTorqCtrl_Handle_t * pHandle )
{
  bool retVal = false;
  if ( pHandle->Mode == STC_SPEED_MODE )
  {
    pHandle->RampRemainingStep = 0u;
    retVal = true;
  }
  return retVal;
}

/**
  * @brief  Force the speed reference to the curren speed. It is used
  *         at the START_RUN state to initialize the speed reference.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval none
  */
void STC_ForceSpeedReferenceToCurrentSpeed( SpeednTorqCtrl_Handle_t * pHandle )
{
  pHandle->SpeedRef01HzExt = ( int32_t )SPD_GetAvrgMecSpeed01Hz( pHandle->SPD ) * ( int32_t )65536;
}

/**
  * @brief  Get the Application minimum negative value of rotor speed. It's
            expressed in tenth of mechanical Hertz.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval uint16_t It returns the application minimum negative value of rotor
            speed expressed in tenth of mechanical Hertz.
  */
int16_t STC_GetMinAppNegativeMecSpeed01Hz( SpeednTorqCtrl_Handle_t * pHandle )
{
  return pHandle->MinAppNegativeMecSpeed01Hz;
}


/** @defgroup  Speed & torque control related module
  * @brief Speed and torque mode configuration related functions
 
@verbatim
 ===============================================================================
              ##### Speed & torque control functions #####
 ===============================================================================
@endverbatim
  * @{
  */

/**
  * @brief  Set the modality of the speed and torque controller. Two modality
  *         are available Torque mode and Speed mode.
  *         In Torque mode is possible to set directly the motor torque
  *         reference or execute a motor torque ramp. This value represents
  *         actually the Iq current reference expressed in digit.
  *         In Speed mode is possible to set the mechanical rotor speed
  *         reference or execute a speed ramp. The required motor torque is
  *         automatically calculated by the STC.
  *         This command interrupts the execution of any previous ramp command
  *         maintaining the last value of Iq.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @param  bMode modality of STC. It can be one of these two settings:
  *         STC_TORQUE_MODE to enable the Torque mode or STC_SPEED_MODE to
  *         enable the Speed mode.
  * @retval none
  */
void STC_SetControlMode( SpeednTorqCtrl_Handle_t * pHandle, STC_Modality_t bMode )
{
  pHandle->Mode = bMode;
  pHandle->RampRemainingStep = 0u; /* Interrupts previous ramp. */
}

/**
  * @brief  Get the modality of the speed and torque controller.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval STC_Modality_t It returns the modality of STC. It can be one of
  *         these two values: STC_TORQUE_MODE or STC_SPEED_MODE.
  */
STC_Modality_t STC_GetControlMode( SpeednTorqCtrl_Handle_t * pHandle )
{
  return pHandle->Mode;
}

/**
  * @brief  Starts the execution of a ramp using new target and duration. This
  *         command interrupts the execution of any previous ramp command.
  *         The generated ramp will be in the modality previously set by
  *         STC_SetControlMode method.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @param  hTargetFinal final value of command. This is different accordingly
  *         the STC modality.
  *         If STC is in Torque mode hTargetFinal is the value of motor torque
  *         reference at the end of the ramp. This value represents actually the
  *         Iq current expressed in digit.
  *         To convert current expressed in Amps to current expressed in digit
  *         is possible to use the formula:
  *         Current(digit) = [Current(Amp) * 65536 * Rshunt * Aop]  /  Vdd micro
  *         If STC is in Speed mode hTargetFinal is the value of mechanical
  *         rotor speed reference at the end of the ramp expressed in tenths of
  *         HZ.
  * @param  hDurationms the duration of the ramp expressed in milliseconds. It
  *         is possible to set 0 to perform an instantaneous change in the value.
  * @retval bool It return false if the absolute value of hTargetFinal is out of
  *         the boundary of the application (Above max application speed or max
  *         application torque or below min application speed depending on
  *         current modality of TSC) in this case the command is ignored and the
  *         previous ramp is not interrupted, otherwise it returns true.
  */
bool STC_ExecRamp( SpeednTorqCtrl_Handle_t * pHandle, int16_t hTargetFinal, uint32_t hDurationms )
{
  bool AllowedRange = true;
  uint32_t wAux;
  int32_t wAux1;
  int16_t hCurrentReference;

  /* Check if the hTargetFinal is out of the bound of application. */
  if ( pHandle->Mode == STC_TORQUE_MODE )
  {
    hCurrentReference = STC_GetTorqueRef( pHandle );
#ifdef CHECK_BOUNDARY
    if ( ( int32_t )hTargetFinal > ( int32_t )pHandle->MaxPositiveTorque )
    {
      AllowedRange = false;
    }
    if ( ( int32_t )hTargetFinal < ( int32_t )pHandle->MinNegativeTorque )
    {
      AllowedRange = false;
    }
#endif
  }
  else
  {
    hCurrentReference = ( int16_t )( pHandle->SpeedRef01HzExt >> 16 );

#ifdef CHECK_BOUNDARY
    if ( ( int32_t )hTargetFinal > ( int32_t )pHandle->MaxAppPositiveMecSpeed01Hz )
    {
      AllowedRange = false;
    }
    else if ( hTargetFinal < pHandle->MinAppNegativeMecSpeed01Hz )
    {
      AllowedRange = false;
    }
    else if ( ( int32_t )hTargetFinal < ( int32_t )pHandle->MinAppPositiveMecSpeed01Hz )
    {
      if ( hTargetFinal > pHandle->MaxAppNegativeMecSpeed01Hz )
      {
        AllowedRange = false;
      }
    }
    else {}
#endif
  }

  if ( AllowedRange == true )
  {
    /* Interrupts the execution of any previous ramp command */
    if ( hDurationms == 0u )
    {
      if ( pHandle->Mode == STC_SPEED_MODE )
      {
        pHandle->SpeedRef01HzExt = ( int32_t )hTargetFinal * 65536;
      }
      else
      {
        pHandle->TorqueRef = ( int32_t )hTargetFinal * 65536;
      }
      pHandle->RampRemainingStep = 0u;
      pHandle->IncDecAmount = 0;
    }
    else
    {
      /* Store the hTargetFinal to be applied in the last step */
      pHandle->TargetFinal = hTargetFinal;

      /* Compute the (wRampRemainingStep) number of steps remaining to complete
      the ramp. */
      wAux = ( uint32_t )hDurationms * ( uint32_t )pHandle->STCFrequencyHz;
      wAux /= 1000u;
      pHandle->RampRemainingStep = wAux;
      pHandle->RampRemainingStep++;

      /* Compute the increment/decrement amount (wIncDecAmount) to be applied to
      the reference value at each CalcTorqueReference. */
      wAux1 = ( ( int32_t )hTargetFinal - ( int32_t )hCurrentReference ) * 65536;
      wAux1 /= ( int32_t )pHandle->RampRemainingStep;
      pHandle->IncDecAmount = wAux1;
    }
  }

  return AllowedRange;
}

/**
  * @brief  Check if the settled speed or torque ramp has been completed.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval bool It returns true if the ramp is completed, false otherwise.
  */
bool STC_RampCompleted( SpeednTorqCtrl_Handle_t * pHandle )
{
  bool retVal = false;
  if ( pHandle->RampRemainingStep == 0u )
  {
    retVal = true;
  }
  return retVal;
}

/**
  * @brief  This command interrupts the execution of any previous ramp command.
  *         If STC has been set in Torque mode the last value of Iq is
  *         maintained.
  *         If STC has been set in Speed mode the last value of mechanical
  *         rotor speed reference is maintained.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval none
  */

void STC_StopRamp( SpeednTorqCtrl_Handle_t * pHandle )
{

  pHandle->RampRemainingStep = 0u;
  pHandle->IncDecAmount = 0;
}

/** @defgroup Torque Control function module
  * @brief  Torque Control component of the Motor Control SDK
 
@verbatim
 ===============================================================================
                   ##### Torque control functions #####
 ===============================================================================
@endverbatim
  * @{
  */

/**
  * @brief  It is used to compute the new value of motor torque reference. It
  *         must be called at fixed time equal to hSTCFrequencyHz. It is called
  *         passing as parameter the speed sensor used to perform the speed
  *         regulation.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval int16_t motor torque reference. This value represents actually the
  *         Iq current expressed in digit.
  *         To convert current expressed in Amps to current expressed in digit
  *         is possible to use the formula:
  *         Current(digit) = [Current(Amp) * 65536 * Rshunt * Aop]  /  Vdd micro
  */
//int16_t STC_CalcTorqueReference( SpeednTorqCtrl_Handle_t * pHandle )
//返回值改为Iqd值
Curr_Components STC_CalcTorqueReference( SpeednTorqCtrl_Handle_t * pHandle )
{
  int32_t wCurrentReference;
  int16_t hTorqueReference = 0;
  int16_t hMeasuredSpeed;
  int16_t hTargetSpeed;
  int16_t hError;
  int32_t temp;
  Curr_Components IqdTemp;
  temp = (( int32_t )pHandle->TargetFinal <<16); //上次扭矩
  hMeasuredSpeed = SPD_GetAvrgMecSpeed01Hz( pHandle->SPD ); //获取当前速度
  if ( pHandle->Mode == STC_TORQUE_MODE )
  {
    wCurrentReference = pHandle->TorqueRef;
  }
  else
  {
    wCurrentReference = pHandle->SpeedRef01HzExt;
  }

  /* Update the speed reference or the torque reference according to the mode
     and terminates the ramp if needed. */
  if ( pHandle->RampRemainingStep > 1u )
  {
    /* Increment/decrement the reference value. */
    if(pHandle->IncDecAmount==0){
      wCurrentReference = temp; //(( int32_t )pHandle->TargetFinal<<16);   //* 65536;
      pHandle->RampRemainingStep = 0u;
    }
    else if(pHandle->IncDecAmount>0){ //增速
      if(wCurrentReference>=temp){
        wCurrentReference = temp; //(( int32_t )pHandle->TargetFinal<<16);   //* 65536;
        pHandle->RampRemainingStep = 0u;
      }
      else if(hMeasuredSpeed<pHandle->TargetFinal){
        //本次速度小 还是需要加速的
        wCurrentReference += pHandle->IncDecAmount;
        pHandle->RampRemainingStep--;
      }else{
        wCurrentReference = temp; //(( int32_t )pHandle->TargetFinal<<16);   //* 65536;
        pHandle->RampRemainingStep = 0u;
      }
    }else{
      if(wCurrentReference<=temp){
        wCurrentReference = temp; //(( int32_t )pHandle->TargetFinal<<16);   //* 65536;
        pHandle->RampRemainingStep = 0u;
      }
      else if(hMeasuredSpeed>pHandle->TargetFinal){
        //本次速度大 还是需要减速
        wCurrentReference += pHandle->IncDecAmount;
        pHandle->RampRemainingStep--;
      }else{
        wCurrentReference = temp; //(( int32_t )pHandle->TargetFinal<<16);   //* 65536;
        pHandle->RampRemainingStep = 0u;
      }
    }
  }
  else if ( pHandle->RampRemainingStep == 1u )
  {
    /* Set the backup value of hTargetFinal. */
    //wCurrentReference = ( int32_t )pHandle->TargetFinal * 65536;
    wCurrentReference = temp; //(( int32_t )pHandle->TargetFinal<<16);   //* 65536;
    pHandle->RampRemainingStep = 0u;
  }
  else
  {
    /* Do nothing. */
  }

  if ( pHandle->Mode == STC_SPEED_MODE )
  {
    /* Run the speed control loop */

    /* Compute speed error */
    hTargetSpeed = ( int16_t )( wCurrentReference >>16 );
    //hMeasuredSpeed = SPD_GetAvrgMecSpeed01Hz( pHandle->SPD );
    if(GetBrakeStatus()==1){
      //中点刹车时候 当前速度如果小于目标速度 直接赋值给目标速度
      if(hTargetSpeed>0){ //目标为正
        if(hTargetSpeed>hMeasuredSpeed){
          //当前小于目标
            hTargetSpeed = hMeasuredSpeed;
        }
      }else if(hTargetSpeed<0){
         if(hTargetSpeed<hMeasuredSpeed){
          //当前小于目标
            hTargetSpeed = hMeasuredSpeed;
        }       
      }
    }
    hError = hTargetSpeed - hMeasuredSpeed;
    //当Iq扭矩已经达到最大了时候不应该再增加误差了 这时已经没用了
    if(hError>0){
      //目标速度>测量速度时候 检测Iq是否已经达到了最大
      if(GetMaxTerefReady()==1){
        hError = 0;
      }
    }else if(hError<0){
      //检测Iq是否已经达到了-最大
      if(GetMaxTerefReady()==2){
        hError = 0;
      }
    }
    if(hTargetSpeed==0){
      //目标速度为0
      if(abs((int)hError)<0x02){
        ErrorStopCount++;
        if(ErrorStopCount>180){
          ErrorStopCount = 181;
          hError = 0;
          //目标不动误差小于阈值超过一定时间认为完全停止了 此时认为电机停止了
          //lastError = 0;
          if(pHandle->PISpeed->wIntegralTerm>0){
            pHandle->PISpeed->wIntegralTerm -= 30;  //8;
            if(pHandle->PISpeed->wIntegralTerm<0){
              pHandle->PISpeed->wIntegralTerm = 0;
            }
          }
          else if(pHandle->PISpeed->wIntegralTerm<0){
            pHandle->PISpeed->wIntegralTerm += 30;  //8;
            if(pHandle->PISpeed->wIntegralTerm > 0){
              pHandle->PISpeed->wIntegralTerm = 0;
            }
          }
        }
      }else{
        //本次误差超过一定范围
        if(((hError>0)&&(LhError<0))||((hError<0)&&(LhError>0))){
          //误差方向不相同
          LhError = hError; //改变方向 积分缩小 1/32
          pHandle->PISpeed->wIntegralTerm -= (pHandle->PISpeed->wIntegralTerm>>3);
        }
        if(ErrorStopCount>0){
          ErrorStopCount -= 4;
          if(ErrorStopCount<0)
            ErrorStopCount = 0;
        }        
      }
    }else{
      //本次目标值不为0 一切正常执行
      ErrorStopCount = 0;
    }
    pHandle->PISpeed->hCompensation = (hTargetSpeed*fw_pro)>>7;  //前馈补偿
    hTorqueReference = PI_Controller( pHandle->PISpeed, ( int32_t )hError );
    pHandle->SpeedRef01HzExt = wCurrentReference;
    //本次速度正 误差负 / 本次速度负 误差正  需要减速
    bool is_brakeing = ((((hError<0)&&(hMeasuredSpeed>0))||((hError>0)&&(hMeasuredSpeed<0)))&&GetISChangeState());
    // 2. 判断母线电压是否超过安全阈值 当电压超过限制电压1.5v时候 -d 到最大20%
    bool is_over_voltage = (GetNowVBus() > GetVBusLimit());
    int16_t weak_Id = 0;  //当前弱磁电流
    if(is_brakeing&&is_over_voltage){
      //当刹车且母线电压大于设定的预置时候有能量回收此时要给-IdRef?
      //-IdRef 最大给到20% 
      uint16_t diff = (GetNowVBus() - GetVBusLimit());
      if(diff>VBusVol(MaxBoostVol)){ //升高对应电压时候对应的最大-Id
        weak_Id = CurrentInt16(MaxWeakId);  //最大-Id
      }else{
        weak_Id = diff*CurrentInt16(MaxWeakId)/VBusTrigAD(MaxBoostVol);
      }
    }
    // 应用一阶低通滤波
    id_weak_field_filtered = id_weak_field_filtered + 
    (((weak_Id - id_weak_field_filtered) * WEAK_FIELD_ALPHA) >> 16);
    IqdTemp.qI_Component2 = id_weak_field_filtered;
    //如果是减速的时候 要限制反向的Iq
    uint16_t speed_abs = abs(hMeasuredSpeed);  // 绝对速度
    //速度 和扭矩方向不一样时候 说明扭矩是负值 此时要限制扭矩
    if(is_brakeing&&(((hMeasuredSpeed>0)&&(hTorqueReference<0))||((hMeasuredSpeed<0)&&(hTorqueReference>0)))){
      //速度大于一定值时候反向Iq要限制,防止-Iq过大 方向升压太高
      int16_t targetLimitIq = 0;  //目标限制-Iq
      if(speed_abs<=SpeedLimit_MIN_NIq_TH){
        targetLimitIq = CurrentInt16(DefaultMaxBrakeCurrent);
      }else if(speed_abs>=SpeedLimit_MAX_NIq_TH){
        targetLimitIq = CurrentInt16(DefaultMinBrakeCurrent);
      }else{
        //targetLimitIq = IQMAX - (speed_abs-SpeedLimit_MIN_NIq_TH)*(IQMAX-IQBrakelimit)/(SpeedLimit_MAX_NIq_TH-SpeedLimit_MIN_NIq_TH)
        int32_t speed_diff = speed_abs - SpeedLimit_MIN_NIq_TH;
        int32_t iq_range = CurrentInt16(DefaultMaxBrakeCurrent); - CurrentInt16(DefaultMinBrakeCurrent);;
        int32_t speed_range = SpeedLimit_MAX_NIq_TH - SpeedLimit_MIN_NIq_TH;
        targetLimitIq = CurrentInt16(DefaultMaxBrakeCurrent) - (int16_t)((speed_diff * iq_range) / speed_range); //线性限制刹车力度
      }
      smoothed_brake_limit += (((targetLimitIq-smoothed_brake_limit)*LIMIT_SMOOTH_ALPHA)>>16);  //本次限制的最大刹车力度
      //本次力度如果超过这个值 就直接限制到这个值
      if((hTorqueReference>smoothed_brake_limit))
        hTorqueReference = smoothed_brake_limit;
        //hTorqueReference = hTorqueReference + 
        //    (((smoothed_brake_limit - hTorqueReference) * LIMIT_SMOOTH_ALPHA) >> 16);
      if((hTorqueReference<-smoothed_brake_limit))
        hTorqueReference = -smoothed_brake_limit;
        //hTorqueReference = hTorqueReference + 
        //    (((-smoothed_brake_limit - hTorqueReference) * LIMIT_SMOOTH_ALPHA) >> 16);
    }
    int16_t tempacc=0;
    int16_t tempacc_abs=0;
    if(is_brakeing){
      //减速时候做斜率判断
      tempacc = ((lastTorq-hTorqueReference)*LIMIT_SubAcc_ALPHA)>>16;  //转矩变化的幅度
      tempacc_abs = abs(tempacc);
      if((lastTorq!=hTorqueReference)){ //转矩不同
        if(tempacc_abs>MaxTorqDecAcc){
          tempacc_abs = MaxTorqDecAcc;
        }else if(tempacc_abs<MinTorqDecAcc){
          tempacc_abs = MinTorqDecAcc;
        }
        if(tempacc<0)
          tempacc_abs = -tempacc_abs;
        lastTorq += tempacc_abs;
      }else{  //转矩一致直接赋值
        lastTorq = tempacc;
      }
    }else{
      //加速直接赋值到转矩
      tempacc = ((lastTorq-hTorqueReference)*LIMIT_AddAcc_ALPHA)>>16;  //转矩变化的幅度
      tempacc_abs = abs(tempacc);
      if((lastTorq!=hTorqueReference)){ //转矩不同
        if(tempacc_abs>MaxTorqAcc){
          tempacc_abs = MaxTorqAcc;
        }else if(tempacc_abs<MinTorqAcc){
          tempacc_abs = MinTorqAcc;
        }
        if(tempacc<0)
          tempacc_abs = -tempacc_abs;
        lastTorq += tempacc_abs;
      }else{  //转矩一致直接赋值
        lastTorq = tempacc;
      }
    }
    pHandle->TorqueRef = ( int32_t )(lastTorq<<16);  //* 65536a;
    IqdTemp.qI_Component1 = hTorqueReference;
    //pHandle->TorqueRef = ( int32_t )hTorqueReference * 65536;
  }
  else
  {
    pHandle->TorqueRef = wCurrentReference;
    hTorqueReference = ( int16_t )( wCurrentReference / 65536 );
    IqdTemp.qI_Component1 = hTorqueReference;
    IqdTemp.qI_Component2 = 0;
  }
  //return hTorqueReference;
  return IqdTemp;
}

/**
  * @brief  Get the current motor torque reference. This value represents
  *         actually the Iq current reference expressed in digit.
  *         To convert current expressed in digit to current expressed in Amps
  *         is possible to use the formula:
  *         Current(Amp) = [Current(digit) * Vdd micro] / [65536 * Rshunt * Aop]
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval int16_t current motor torque reference. This value represents
  *         actually the Iq current expressed in digit.
  */
int16_t STC_GetTorqueRef( SpeednTorqCtrl_Handle_t * pHandle )
{
  return ( ( int16_t )( pHandle->TorqueRef / 65536 ) );
}

/**
  * @brief It returns the default values of Iqdref.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @retval default values of Iqdref.
  */
Curr_Components STC_GetDefaultIqdref( SpeednTorqCtrl_Handle_t * pHandle )
{
  Curr_Components IqdRefDefault;
  IqdRefDefault.qI_Component1 = pHandle->TorqueRefDefault;
  IqdRefDefault.qI_Component2 = pHandle->IdrefDefault;
  return IqdRefDefault;
}

/**
  * @brief  Change the nominal current.
  * @param  pHandle: handler of the current instance of the SpeednTorqCtrl component
  * @param  hNominalCurrent This value represents actually the maximum Iq current
            expressed in digit.
  * @retval none
  */
void STC_SetNominalCurrent( SpeednTorqCtrl_Handle_t * pHandle, uint16_t hNominalCurrent )
{
  pHandle->MaxPositiveTorque = hNominalCurrent;
  pHandle->MinNegativeTorque = -hNominalCurrent;
}


/**
  * @}
  */

/**
  * @}
  */

