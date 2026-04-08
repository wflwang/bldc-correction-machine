/**
 * @file app.h
 * 
 * 
 */

#ifndef __APP_H
#define __APP_H

#include <stdint.h>
#include <stdbool.h>
#include "hk32m07x.h"
#include "main.h"


#define PPMMinLimit 700
#define PPMMaxLimit 2300


#define duplex  //开启双向模式

#define MOTOR_Min_SPEED_RPM      15  //8 //15 //15 //@10 //3  //5  //1
#define PPM_MAX_V    2000
#define PPM_MAX      1950
#define PPM_MIN_V    1000
#define PPM_MIN      1050
#define PPM_MID_V    1500
#define PPM_MID_UP   1525  //1517  //1580
#define PPM_MID_DOWN 1485  //1420
#define PPM_MAXLVL   1670  //1650   //快慢速度分界
#define PPM_MINLVL   1330  //1350

#define MidDeadPPM      25
#define MaxDeadPPM      25

#define DefPPMRampUP    10
#define DefPPMRampDown  10

#define ppmdir  0   //PPM 方向 控制电机正反转

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum{
    APP_Null = 0,
    APP_PPM,
    APP_Uart,
    APP_PWM,
}app_mode_t;


typedef struct{
    bool NoRev;     //没有后退 单向模式
    uint8_t PPM_Dir;   //PPM值得方向
    uint8_t PPMStep;    //学习步骤
    int16_t MaxPPM; //PPM 最大点
    int16_t MidPPM; //PPM中点
    int16_t MinPPM; //PPM最小点
    int16_t MidPPMdead;    //PPM中点范围 左右多少死区范围
    int16_t MaxPPMdead;     //最大点死区
    int16_t MinPPMdead;     //最小点死区
    uint16_t RampUp;         //PPM从0到最大的时间  单位20ms
    uint16_t RampDown;       //PPM从0到最小的时间 单位20ms 变化一次
    int16_t (*GetNowPPM)(void); //获取当前PPM
    bool (*GetPPMLost)(void);     //获取链接状态
    int32_t (*GetMaxSpeed)(void);   //最大速度
}app_PPM_t;


typedef struct{
    app_mode_t app_mode;
    app_PPM_t app_PPM;
    int16_t CRC_Data; //数据校验
}app_config_t;



int16_t GetPPMValue(void);
bool GetPWMLost(void);
uint32_t GetPPMHTime(void);
int UPDATA_SPEED(app_PPM_t *apm);   //获取目标速度
uint8_t GetPPMUpdate(void);
void ClrPPMUpDate(void);





#ifdef __cplusplus
}
#endif /* __cpluplus */
#endif