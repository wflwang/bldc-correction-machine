/** 
 * @file PPM control
 * 
 * 
 * @example 选择单向双向 速度控制/duty控制 PPM中点 正最大/负最大?
*/

#include "app.h"
#include <stdint.h>
//#include <stdint.h>

/**
 * @brief 设置PPM的最大最小范围
 * 
 * 
 * 
*/
#if 0
static void SetPPMRange(app_PPM_t *apm){
  int16_t plus = apm->GetNowPPM();
  switch (apm->PPMStep)
  {
    case 0:
      if((plus<1600)&&(plus>1400)){
        apm->MidPPM = plus;
        apm->PPMStep++;
        //GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        //speechEn(70,2);   //学习到终点响一声
      }
    break;
    case 1:
      if(plus<(PPM_MINLVL-50)){
        apm->MinPPM = plus;
        apm->PPMStep++;
        //GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        //speechEn(70,2);   //学习到最小值 响一声
        //PPM_valueDir = 1;
      }else if(plus>(PPM_MAXLVL+50)){
        apm->MaxPPM = plus;
        apm->PPMStep++;
        //GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        //speechEn(70,2);   // 学习到最大值响一声
        //PPM_valueDir = 0;
      }
    break;
    default:
      //if(PPM_valueDir==0){
        if(plus<(PPM_MINLVL-50)){
          apm->MinPPM = plus;
          apm->PPMStep++;
          //GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
          //speechEn(70,2);   //结束常响一声
        }
      //}else{
        if(plus>(PPM_MAXLVL+50)){
        apm->MaxPPM = plus;
        apm->PPMStep++;
        //GPIO_SetBits(PIN_RunLampPort,PIN_RunLampBit);
        //speechEn(500,1);   //
        }
      //} 
    break;
  }
}
#endif
/**
  * @brief updata VR speed
  *  这个函数为了增加 Ramp 的功能 不能调用太快 ~=20ms 一次 
  * @retval None
  */
int UPDATA_SPEED(app_PPM_t *apm)
{
  static int lastOut=0;  //上次的速度
  int out;
  int16_t maxs = apm->GetMaxSpeed();	//获取最大允许转速
  int16_t plus = apm->GetNowPPM();
  //if(maxs<MOTOR_Mid_SPEED_RPM)  //最小转速限制
  //  maxs = MOTOR_Mid_SPEED_RPM;
//#ifdef duplex  仅仅支持双向转
  uint16_t PPM_midUp = apm->MidPPM+apm->MidPPMdead;
  uint16_t PPM_midDown = apm->MidPPM-apm->MidPPMdead;  //PPM_MID_DOWN
  uint16_t PPM_maxLimit = apm->MaxPPM-apm->MaxPPMdead; //PPM_MAX
  uint16_t PPM_minLimit = apm->MinPPM+apm->MinPPMdead; // PPM_MIN
  //uint8_t ct_dir = 0; //做前后转刹车用 也用不上
  int maxAdd = maxs / apm->RampUp; //每次最大增加的速度大小
  int maxDec = maxs / apm->RampDown; //每次最大减小的速度大小
  if(maxAdd<1)
    maxAdd = 1;
  if(maxDec<1)
    maxDec = 1;
  if(apm->GetPPMLost()==false)
    out = 0;
  else if(maxs<MOTOR_Min_SPEED_RPM)
    out =  0;	//最大速度小于最小速度直接输出0
  else if(plus<PPMMinLimit)
    out = 0;  //plus 不可能太小
  else if(plus>PPM_midUp){  //PPM_MID_UP
    if(plus>PPM_maxLimit){ //PPM_MAX
      out = maxs;
    }else
    //速度做完全线性 放弃攀爬车更长得低速区域
    out =  ((plus - PPM_midUp)*(maxs-MOTOR_Min_SPEED_RPM)/(PPM_maxLimit - PPM_midUp)+MOTOR_Min_SPEED_RPM);  //DEFAULT_TARGET_SPEED_RPM;
  }else if(plus<PPM_midDown){
    //maxs >>= 2;
    //#ifdef backslow
    //if(PPM_valueDir==0)
    //maxs = (maxs*204)>>8;
    //#endif
    //if(PPM_valueDir==0)
    //  ct_dir = 1;
    if(plus<PPM_minLimit){
      out = -maxs;
      //return -maxs;
    }else
    out =  -((PPM_midDown - plus)*(maxs-MOTOR_Min_SPEED_RPM)/(PPM_midDown - PPM_minLimit)+MOTOR_Min_SPEED_RPM); //DEFAULT_TARGET_SPEED_RPM;
  }else{
    out = 0;
  }
  if(out>lastOut){
    //本次值大于上次值 是增加速度
    out -= lastOut;  //本次和上次的差速
    if(out>maxAdd)
      out = maxAdd;
   lastOut += out; 
  }else if(out<lastOut){
    //是减小速度
    out = lastOut - out;  
    if(out>maxDec)
      out = maxDec;
   lastOut -= out; 
  }
  if(apm->PPM_Dir==0)
    return lastOut;
  else{
    return -lastOut;
  }
//#else //单向转
//  if(plus<1200){
//    return 0;
//  }else if(plus>1900){
//    return MOTOR_MAX_SPEED_RPM;
//  }else{
//    return ((plus-1200)*(MOTOR_MAX_SPEED_RPM-DEFAULT_TARGET_SPEED_RPM)/700 + DEFAULT_TARGET_SPEED_RPM);
//  }
//#endif
}
