/** 
 * @file app control
 * 
 * 
 * @example 选择单向双向 速度控制/duty控制 PPM中点 正最大/负最大?
*/

#include "app.h"
#include "utils_math.h"
#include <cstdint>
#include "mc_tasks.h"

#define lostPWMCnt 2       //连接阈值 大于这个数才是连上否则都是掉码
static uint32_t PWM_L=0,PWM_H=0;
static int cnt=0;
static int16_t lastPPM = 0;
static uint8_t updateReady=0;   //有更新标志

/**
  * @brief  This function handles UTU1 Handler.
  * @param  None
  * @retval None
  */
void UTU1_IRQHandler(void)
{
    uint32_t temp;
    temp = UTU1->UIFR;
    if (temp & UTU_IT_UCRA)
    {
		//上升沿中断
        PWM_L = UTU_GetCapture(UTU1, UTU_ICSelection_IO_SELF);
        //响应了一个完整的周期
        if(cnt<lostPWMCnt)
	    cnt++;
    }
	if(temp & UTU_IT_UCFA){
		//下降沿中断 有数据更新
        updateReady = 1;    //有更新
        PWM_H = UTU_GetCapture(UTU1, UTU_ICSelection_IO_SELF);
	}
	if(temp & UTU_IT_OVF){
		//最大时间溢出了 可能是最大 也可能是最小
        //超时也是完整周期
        updateReady = 1;    //有更新
        PWM_H = 0;
        lastPPM = 0;        //掉码
		cnt = 0;
	}
    switch(GetAPPMode()){
        case APP_Null:
        break;
        case APP_PPM:
            if((PWM_H>PPMMinLimit)&&(PWM_H<PPMMaxLimit)){
                if(cnt==lostPWMCnt){
                    cnt++;
                    lastPPM = PWM_H;
                }
                if(cnt>lostPWMCnt){
                    //在范围内且计时达到可以输出结果
                    UTILS_LPInt16_FAST(lastPPM,PWM_H,(int16_t)(0.3*32767));
                }else{
                    lastPPM = 0;        //掉码
                }
            }   //不在范围内也认为是错误的 但是输出上次的值
        break;
        case APP_Uart:
        break;
        case APP_PWM:
			lastPPM = (PWM_L<<15) / PWM_H;
        break;
    }
    UTU_ClearFlag(UTU1, (uint16_t)temp);
}
/**
 * @brief 获取本次PPM的值
 * 
 * 
*/
int16_t GetPPMValue(void){
    return lastPPM;
}

/**
 * @brief get High time
 * 
 * 
*/
uint32_t GetPPMHTime(void){
    return PWM_H;
}
/**
 * @brief lost?
 * 
 * @return true:lost false: connect
*/
bool GetPWMLost(void){
    if(cnt<lostPWMCnt)
    return true;
    else
    return false;
}

/**
 * @brief get app mode?
 * 
 * 
 */
uint8_t GetPPMUpdate(void){
    return updateReady;
}
void ClrPPMUpDate(void){
    updateReady = 0;
}

