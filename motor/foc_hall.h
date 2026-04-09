/**
 * @file foc_hall.h
 * @author MaxwellWang
 * 
 * 
 */

#ifndef FOC_HALL_H_
#define FOC_HALL_H_

//#include   "datatypes.h"
#include "stdint.h"
#include "main.h"
#include "mc_type.h"
#include "speed_pos_fdbk.h"
#include "speed_torq_ctrl.h"
#include "virtual_speed_sensor.h"
//#include <cstdint>

//24bit = 262ms
#define MaxAng60IntTime 0x4000000uL   //60度时间默认最大值 65536*8us=524ms 认为是堵转了
typedef struct
{
    SpeednPosFdbk_Handle_t _Super;
    SpeednTorqCtrl_Handle_t * pSTC;       /*!< Speed and torque controller object used by  EAC.*/
    VirtualSpeedSensor_Handle_t * pVSS;   /*!< Virtual speed sensor object used by EAC.*/
    hall_state_t hallState;     //hall状态
    bool angUpdate;     //hall角度更新了 可以用来更新最新角度了
    //bool EncAligned;   /*!< This flag is true if the encoder has been \
                               aligned at least one time, false if hasn't been\
                                never aligned.*/
    bool I_feed; //进入电流环标志
    uint8_t hall_val;       //当前hall位置指针 0-7
    int16_t intTime;      //中断时间 ?*1/64us
    int16_t m_ang_hall_int_prev;   //上次hall保存的角度
    int16_t m_ang_hall_int_Next;   //下次hall预测的角度 hall中断中计算的值
    int16_t real_phase_Next;        //下次hall预测的角度 AD中断中实际的值
    int16_t anginc;    //每次中断变化的角度 相当于角速度*中断时间
    int16_t real_phase;     //hall 应用的真实角度
    int16_t last_ang_diff;     //上次hall角度误差 主要是为了判断误差变化方向
    //int16_t foc_hall_tableTemp[8];  //hall学习时候临时记录的表格 0-7
    int16_t *foc_hall_table;  //hall学习完成后正式使用的表格 0-7
    int16_t hallFastLearnAngDiff; //hall学习时候可以快速跳过的角度增量 增加学习效率
    uint32_t m_ang60_intTime;   //60度时间
    //bool bElDiffMec;  //马达电角度方向是否和磁编角度不同
    //bool hDirFin; //学习的时候是否识别到了方向
    //bool NegDir;
    uint16_t hRemainingTicks;             /*!< Number of clock events remaining to complete\
                                             the alignment.*/
    uint16_t hEACFrequencyHz; /*!< Frequency expressed in Hz at which the user\
                                 clocks the EAC calling EAC_Exec method */
    int16_t hFinalTorque;     /*!< Motor torque reference imposed by STC at the\
                                 end of programmed alignment. This value\
                                 represents actually the Iq current expressed in\
                                 digit.*/
    int16_t hElAngle;        /*!< 对齐时候的角度 Electrical angle of programmed alignment\
                                 expressed in s16degrees.*/
    //int8_t offsetCount;  //校准次数  要校准两个极对
    //int16_t offsetElAngle;  //对齐后的角度偏移
    //int16_t offsetElAngle1;  //对齐后另一个极对的角度偏移 
    //int16_t offsetElAngleTemp;  //对齐后90度的脚的角度偏移
    uint16_t SpeedSamplingFreq01Hz; /*!< Frequency (01Hz) at which motor speed is to be
                                   computed. It must be equal to the frequency
                                   at which function SPD_CalcAvrgMecSpeed01Hz
                                   is called.*/
    uint16_t hDurationms;     /*!< Duration of the programmed alignment expressed
                                 in milliseconds.*/
    //uint8_t bElToMecRatio;  //极对数
    int16_t feed_v;  //反馈加权系数
    //uint8_t bBatNum;  //电池数量 不同电池数量 最大速度不一样?
    //uint8_t lowVDDflag; //低压标志
    //uint8_t TempOverflag; //过温保护标志
    uint16_t Maxspeed;  //最大速度
    uint16_t NowVBus; //当前母线电压
    uint16_t MaxLimitVBus;  //最大限制母线电压 超过就要弱磁
    //uint16_t tabAngleIndex;  //表格位置
    //uint16_t tabAnglePage;  //表格page位置 128*16 = 2048 = 2*1024 全角度 128int16 = 256int8
    //uint16_t tabMinAngIdx;  //定位到的指针
    //volatile int32_t DeltaCapturesBuffer[16]; /*!< Buffer used to store \
                                        captured variations of timer counter*/
    //volatile uint8_t DeltaCapturesIndex; /*! <Buffer index*/
    //volatile uint32_t lastMecAngle; //上次的物理角度
    //#ifdef avfilter
    //volatile int32_t SumMecAngle; //角度增量的和
    //#else
    //volatile int32_t SumMecAngle; //角度增量的和
    //#endif
    //uint32_t *tabAngle; //存储角度的指针
} foc_hall_t;


void M_Hall_Init(foc_hall_t * pHandle,mc_config_t *mcconf);
/**
 * @brief get hall state
 * 
 * 
 */
static inline hall_state_t GetHallState(foc_hall_t * pHandle){
    return pHandle->hallState;
}
int16_t GetLastLearnAngDiff(foc_hall_t * pHandle);




#endif
