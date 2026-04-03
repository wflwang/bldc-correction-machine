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
    int16_t MaxPPM; //PPM 最大点
    int16_t MidPPM; //PPM中点
    int16_t MinPPM; //PPM最小点
    int16_t MidPPMdead;    //PPM中点范围 左右多少死区范围
    int16_t MaxPMMdead;     //最大点死区
    int16_t MinPPMdead;     //最小点死区
    int16_t RampUp;         //PPM从0到最大的时间  单位ms
    int16_t RampDown;       //PPM从0到最小的时间 单位ms
    int16_t CRC_Data; //数据校验
}app_PPM_t;


typedef struct{
    app_mode_t app_mode;
    app_PPM_t app_PPM;
}app_config_t;






#ifdef __cplusplus
}
#endif /* __cpluplus */
#endif