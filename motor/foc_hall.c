/**
 * @file foc_hall.c
 * @brief hall correct
 * @param 
 * @param
 * @retval hall_angle 返回霍尔的角度
 */

#include "foc_hall.h"
#include "hk32m07x.h"

static volatile uint32_t m_ang60_intTime=0;  //60度换向时间单位是 1/8us 最长 512s

/**
 * @brief hall IRQ
 * 
 * 
 */
void HTU_IRQHandler(void){
    /* HALL Timer Update IT always enabled, no need to check enable UPDATE state */
    if (((HTU->HIFR & (uint16_t)HTU_IT_OVF) != (uint16_t)RESET)) 
    {
        //hall 计数器溢出中断
        HTU->HIFR  = (uint16_t)HTU_IT_OVF;
        HALL_TIMx_UP_IRQHandler(&HALL_M1);
    }

    /* HALL Timer CC1 IT always enabled, no need to check enable CC1 state */
    if (((HTU->HIFR & (uint16_t)HTU_IT_SW) != (uint16_t)RESET))  
    {
        //数据变化中断
        HTU->HIFR  = (uint16_t)HTU_IT_SW;
        HALL_TIMx_CC_IRQHandler(&HALL_M1);
    }    
}

#if defined (CCMRAM)
    #if defined (__ICCARM__)
        #pragma location = ".ccmram"
    #elif defined (__CC_ARM) || defined(__GNUC__)
        __attribute__( ( section ( ".ccmram" ) ) )
    #endif
#endif
/**
* @brief  Example of private method of the class HALL to implement an MC IRQ function
*         to be called when TIMx capture event occurs
* @param  pHandle: handler of the current instance of the hall_speed_pos_fdbk component
* @retval none
* 进来算hall table
*/
void * HALL_TIMx_CC_IRQHandler( void * pHandleVoid )
{
    HALL_Handle_t * pHandle = ( HALL_Handle_t * ) pHandleVoid;
    int ang_diff;   //两次有效的角度偏差
    uint8_t bPrevHallState;
    uint32_t wCaptBuf;
    uint16_t hPrscBuf;
    uint32_t hHighSpeedCapture;
    //霍尔值
    uint8_t hall_val = GPIO_IsInputPinSet( pHandle->H3Port, pHandle->H3Pin ) << 2
                        | GPIO_IsInputPinSet( pHandle->H2Port, pHandle->H2Pin ) << 1
                        | GPIO_IsInputPinSet( pHandle->H1Port, pHandle->H1Pin );
    //对应出不同角度值 0,1,2,3,4,5,6,7
    //每次进来比较本次和上次的时间误差
    // 角度变化/时间误差 = 当前变化的角速度 *dt(中断时间) = 每次中断预计变化的角度
    // 如果变化的角度可能会超过下一次的位置 但实际没有 就没变变化1/2角度等待新的到来 如果等待次数超过认为hall error
    //如果变化的数据不是正常的数据 霍尔检测错误, 则
    // 用当前的角速度 每次中断估算变化的角度
    // hall_speed 也要做一阶平滑PI控制
    //60度换向时间
    //每次霍尔变化的时间
    m_ang60_intTime += HTU_GetWBRValue(HTU_WBR0);  //获取本次误差的时间
    if(m_ang60_intTime>MaxAng60IntTime){    //超过最长换向时间 就报错
        m_ang60_intTime = MaxAng60IntTime;
        motor->m_ang_hall_int_prev = 65536; //给一个错误的角度
    }
    if((hall_val==0)||(hall_val==7)){
        //如果hall 值是0/7 不应该出现的值 可能是速度太快霍尔检测错误 120度安装
        //此时错误不算时间 也不更新速度,下次计数应该用错误计数的值和下次值的累加和
        return; //不计算速度和时间
    }
    motor->hall_val = hall_val; //输出新的角度,主程序中可以知道角度的更新 方便映射电角度
    //这里加入学习模式和正常模式区别
    //学习模式 每次触发正确的变化就学习一次,学习模式正反转都要学习
    //开环时候自动学习?
    //正常模式每次触发就计算一次角度和速度
    //霍尔角度设定在0-0xffff(0-65535)之间
    //
    int ang_hall_int = conf_now->foc_hall_table[hall_val];	//当前值对应表格位置的角度值
    //正常角度是在 -32768(0x8000) ~ 32767（0x7fff)
    //本次只更新角度切换点 实际角度在AD中断中去判断
    //这里只算出当前hall切换点角度和当前hall角速度
    //不考虑角度不对情况
    if(ang_hall_int<65536){
        if(motor->m_ang_hall_int_prev>65535){   //如果上次角度不对
            //上次是否角度不正常 不正常直接赋值正常角度
            //从误差过来计算角速度时间默认最大
            motor->angWspeed = MaxAngWSpeed; //第一次默认角速度时间最长? 基本没有补偿
            motor->m_ang_hall_int_prev  = ang_hall_int; //本次角度直接更新
        }else if(ang_hall_int != motor->m_ang_hall_int_prev){
            //角度变化不一样 上次和本次角度都是有效 才可以更新角度,计算出角速度
            //    ang/m_ang60_intTime   角度/时间  = 角速度
            //必须要检测角度变化最大 范围 不可超过最大范围
            ang_diff = ang_hall_int - motor->m_ang_hall_int_prev;
            //hall 变化超过一半的角度变化认识是反转 直接减去65536
            if(ang_diff>32767){
                ang_diff -= 65536
            }else if(ang_diff<-32768){
                ang_diff += 65536
            }
            //误差变化方向是否一致 一致时候计算变化时间才有效 否则用上次60度时间
            if(SIGN(ang_diff) == SIGN(motor->last_ang_diff)){
                //误差方向一致
                if(ang_diff>0){
                    //误差变大
                    motor->m_ang60_intTime = m_ang60_intTime;
                }else{
                    //误差变小 时间增量为-
                    motor->m_ang60_intTime = -m_ang60_intTime;
                }
            }else{
                motor->last_ang_diff = ang_diff;    //更新误差
                motor->m_ang60_intTime = -motor->m_ang60_intTime; //如果角度不同的话用上次,防止正反转误差  
            }
        }
        motor->m_ang_hall_int_prev  = ang_hall_int; //本次角度直接更新
        m_ang60_intTime = 0;    //重新更新下一次时间
        //如果本次转速小于最小插值速度的阈值了,不进行插值 直接用就近角度
        //插值直接处理还是在AD中和无感观测速度一起处理?
        //补偿源用当前主观测器的速度
    }else{
        //角度数据不对
        m_ang60_intTime = 0;    //重新计时
        motor->m_ang_hall_int_prev = 65536; //给一个错误的角度
        return; //
    }
}

#if defined (CCMRAM)
    #if defined (__ICCARM__)
        #pragma location = ".ccmram"
    #elif defined (__CC_ARM) || defined(__GNUC__)
        __attribute__( ( section ( ".ccmram" ) ) )
    #endif
#endif
/**
* @brief  Example of private method of the class HALL to implement an MC IRQ function
*         to be called when TIMx update event occurs
* @param  pHandle: handler of the current instance of the hall_speed_pos_fdbk component
* @retval none
*/
void * HALL_TIMx_UP_IRQHandler( void * pHandleVoid )
{
    HALL_Handle_t * pHandle = ( HALL_Handle_t * ) pHandleVoid;

    if ( pHandle->SensorIsReliable )
    {
        uint16_t hMaxTimerOverflow;
        /* an update event occured for this interrupt request generation */
        pHandle->OVFCounter++;

        hMaxTimerOverflow = ( uint16_t )( ( ( uint32_t )pHandle->HallTimeout * pHandle->OvfFreq ) / 1000u );

        if ( pHandle->OVFCounter >= hMaxTimerOverflow )
        {
            /* Set rotor speed to zero */
            pHandle->_Super.hElSpeedDpp = 0;

            /* Reset the electrical angle according the hall sensor configuration */
            HALL_Init_Electrical_Angle( pHandle );

            /* Reset the overflow counter */
            pHandle->OVFCounter = 0u;


#if 1
            /* Reset the SensorSpeed buffer*/
            uint8_t bIndex;

            for ( bIndex = 0u; bIndex < pHandle->SpeedBufferSize; bIndex++ )
            {
                pHandle->SensorSpeed[bIndex]  = 0;
            }

            pHandle->BufferFilled = 0 ;
            pHandle->CurrentSpeed = 0;
            pHandle->SpeedFIFOIdx = 1;
            pHandle->ElSpeedSum = 0;
#else
            pHandle->ElSpeedSum -= pHandle->SensorSpeed[pHandle->SpeedFIFOIdx];
            pHandle->SensorSpeed[pHandle->SpeedFIFOSetIdx]  = 0;
            pHandle->CurrentSpeed = 0;
            pHandle->SpeedFIFOIdx++;

            if ( pHandle->SpeedFIFOIdx == pHandle->SpeedBufferSize )
            {
                pHandle->SpeedFIFOIdx = 0u;
            }

#endif
        }
    }

    return MC_NULL;
}

/**
 * @brief correct hall
 * 
 * @retval hall_angle
 */
int16_t foc_correct_hall(void){

}