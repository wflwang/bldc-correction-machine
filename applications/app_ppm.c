/** 
 * @file PPM control
 * 
 * 
 * @example 选择单向双向 速度控制/duty控制 PPM中点 正最大/负最大?
*/

#include "app.h"

/**
 * @brief 设置PPM的最大最小范围
 * 
 * 
 * 
*/
static void SetPPMRange(void){
  switch (PPMStep)
  {
    case 0:
      if((vplusT<1600)&&(vplusT>1400)){
        PPM_mid = vplusT;
        PPMStep++;
        GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        speechEn(70,2);   //学习到终点响一声
      }
    break;
    case 1:
      if(vplusT<(PPM_MINLVL-50)){
        PPM_min = vplusT;
        PPMStep++;
        GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        speechEn(70,2);   //学习到最小值 响一声
        PPM_valueDir = 1;
      }else if(vplusT>(PPM_MAXLVL+50)){
        PPM_max = vplusT;
        PPMStep++;
        GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        speechEn(70,2);   // 学习到最大值响一声
        PPM_valueDir = 0;
      }
    break;
    default:
      if(PPM_valueDir==0){
        if(vplusT<(PPM_MINLVL-50)){
          PPM_min = vplusT;
          PPMStep++;
          GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
          //speechEn(70,2);   //结束常响一声
        }
      }else{
        if(vplusT>(PPM_MAXLVL+50)){
        PPM_max = vplusT;
        PPMStep++;
        GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        //speechEn(500,1);   //
        }
      } 
    break;
  }
}
/**
  * @brief updata VR speed
  * @retval None
  */
int UPDATA_SPEED(uint16_t plus)
{
  int out;
  int maxs = (int)GetMaxSpeed();	//获取最大允许转速
  if(maxs<MOTOR_Mid_SPEED_RPM)
    return 0;	//最大速度小于最小速度直接输出0
  //if(maxs<MOTOR_Mid_SPEED_RPM)  //最小转速限制
  //  maxs = MOTOR_Mid_SPEED_RPM;
#ifdef duplex
  uint16_t PPM_midUp = PPM_mid+25;
  uint16_t PPM_midDown = PPM_mid-20;  //PPM_MID_DOWN
  uint16_t PPM_maxLimit = PPM_max-25; //PPM_MAX
  uint16_t PPM_minLimit = PPM_min-25; // PPM_MIN
  ct_dir = 0;
  if(plus>PPM_midUp){  //PPM_MID_UP
    #ifdef backslow		//后退速度减慢
    if(PPM_valueDir==1)
    maxs = (maxs*204)>>8;
    #endif
    if(PPM_valueDir==1)
      ct_dir = 1;
    if(plus>PPM_maxLimit){ //PPM_MAX
      out = maxs;
      //return maxs;
    }
    else if(plus>PPM_MAXLVL){
      out =  ((plus - PPM_MAXLVL)*(maxs-MOTOR_Mid_SPEED_RPM)/(PPM_maxLimit - PPM_MAXLVL)+MOTOR_Mid_SPEED_RPM);  //DEFAULT_TARGET_SPEED_RPM;
      //return ((plus - PPM_MAXLVL)*(maxs-MOTOR_Mid_SPEED_RPM)/(PPM_MAX - PPM_MAXLVL)+MOTOR_Mid_SPEED_RPM);  //DEFAULT_TARGET_SPEED_RPM;
    }else if(plus<(PPM_midUp+10)){
      out =  MOTOR_Min_SPEED_RPM;  //DEFAULT_TARGET_SPEED_RPM;
      //return MOTOR_Min_SPEED_RPM;  //DEFAULT_TARGET_SPEED_RPM;
    }else{
      out =  ((plus - (PPM_midUp+10))*(MOTOR_Mid_SPEED_RPM-MOTOR_Min_SPEED_RPM)/(PPM_MAXLVL - (PPM_midUp+10))+MOTOR_Min_SPEED_RPM);  //DEFAULT_TARGET_SPEED_RPM;
      //return ((plus - (PPM_MID_UP+10))*(MOTOR_Mid_SPEED_RPM-MOTOR_Min_SPEED_RPM)/(PPM_MAXLVL - (PPM_MID_UP+10))+MOTOR_Min_SPEED_RPM);  //DEFAULT_TARGET_SPEED_RPM;
    }
  }else if(plus<PPM_midDown){
    //maxs >>= 2;
    #ifdef backslow
    if(PPM_valueDir==0)
    maxs = (maxs*204)>>8;
    #endif
    if(PPM_valueDir==0)
      ct_dir = 1;
    if(plus<PPM_minLimit){
      out = -maxs;
      //return -maxs;
    }
    else if(plus<PPM_MINLVL){
      out =  -((PPM_MINLVL - plus)*(maxs-MOTOR_Mid_SPEED_RPM)/(PPM_MINLVL - PPM_minLimit)+MOTOR_Mid_SPEED_RPM); //DEFAULT_TARGET_SPEED_RP
      //return -((PPM_MINLVL - plus)*(maxs-MOTOR_Mid_SPEED_RPM)/(PPM_MINLVL - PPM_MIN)+MOTOR_Mid_SPEED_RPM); //DEFAULT_TARGET_SPEED_RPM;
    }else if(plus>(PPM_midDown-10)){
      out =  -MOTOR_Min_SPEED_RPM; //DEFAULT_TARGET_SPEED_RPM;
      //return -MOTOR_Min_SPEED_RPM; //DEFAULT_TARGET_SPEED_RPM;
    }else{
      out =  -(((PPM_midDown-10) - plus)*(MOTOR_Mid_SPEED_RPM-MOTOR_Min_SPEED_RPM)/((PPM_midDown-10) - PPM_MINLVL)+MOTOR_Min_SPEED_RPM); //DEFAULT_TARGET_SPEED_RPM;
      //return -(((PPM_MID_DOWN-10) - plus)*(MOTOR_Mid_SPEED_RPM-MOTOR_Min_SPEED_RPM)/((PPM_MID_DOWN-10) - PPM_MINLVL)+MOTOR_Min_SPEED_RPM); //DEFAULT_TARGET_SPEED_RPM;
    }
  }else{
    out = 0;
    //return 0;
  }
  if(PPM_Dir==0)
    return out;
  else{
    return -out;
  }
#else
  if(plus<1200){
    return 0;
  }else if(plus>1900){
    return MOTOR_MAX_SPEED_RPM;
  }else{
    return ((plus-1200)*(MOTOR_MAX_SPEED_RPM-DEFAULT_TARGET_SPEED_RPM)/700 + DEFAULT_TARGET_SPEED_RPM);
  }
#endif
}