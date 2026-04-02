/**
 * @file foc_hall.h
 * @author MaxwellWang
 * 
 * 
 */

#ifndef FOC_HALL_H_
#define FOC_HALL_H_

//#include   "datatypes.h"
#include "main.h"
#include <cstdint>

//24bit = 262ms
#define MaxAng60IntTime 0x4000000uL   //60度时间默认最大值 65536*8us=524ms 认为是堵转了



void M_Hall_Init(void);


#endif