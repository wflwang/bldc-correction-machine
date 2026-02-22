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
    motor->hall_val = hall_val; //输出新的角度
    //这里加入学习模式和正常模式区别
    //学习模式 每次触发正确的变化就学习一次,学习模式正反转都要学习
    //开环时候自动学习?
    //正常模式每次触发就计算一次角度和速度
    //霍尔角度设定在0-0xffff(0-65535)之间
    //
    int ang_hall_int = conf_now->foc_hall_table[hall_val];	//当前值对应表格位置的角度值
    //正常角度是在 -32768(0x8000) ~ 32767（0x7fff)
    if(ang_hall_int<65536){
        if(motor->m_ang_hall_int_prev>65535){
        //上次是否角度不正常 不正常直接赋值正常角度
            motor->m_ang_hall_int_prev  = ang_hall_int; //本次角度直接更新
        }else if(ang_hall_int != motor->m_ang_hall_int_prev){
        //角度变化不一样
        }else{
        //角度变化一样 不做处理
        }
        m_ang60_intTime  = 0; //角速度第一次补偿到最慢速度时间
    }else{
        //角度数据不对
        m_ang60_intTime = 0;
        motor->m_ang_hall_int_prev = 65536; //给一个错误的角度
        return; //
    }
    if ( pHandle->SensorIsReliable )
    {
        /* A capture event generated this interrupt */
        bPrevHallState = pHandle->HallState;

        if ( pHandle->SensorPlacement == DEGREES_120 )
        {
            pHandle->HallState  = GPIO_IsInputPinSet( pHandle->H3Port, pHandle->H3Pin ) << 2
                                  | GPIO_IsInputPinSet( pHandle->H2Port, pHandle->H2Pin ) << 1
                                  | GPIO_IsInputPinSet( pHandle->H1Port, pHandle->H1Pin );
        }
        else
        {
            pHandle->HallState  = ( GPIO_IsInputPinSet( pHandle->H2Port, pHandle->H2Pin ) ^ 1 ) << 2
                                  | GPIO_IsInputPinSet( pHandle->H3Port, pHandle->H3Pin ) << 1
                                  | GPIO_IsInputPinSet( pHandle->H1Port, pHandle->H1Pin );
        }

        switch ( pHandle->HallState )
        {
            case STATE_5:
                if ( bPrevHallState == STATE_4 )
                {
                    pHandle->Direction = POSITIVE;
                    pHandle->MeasuredElAngle = pHandle->PhaseShift;
                }
                else if ( bPrevHallState == STATE_1 )
                {
                    pHandle->Direction = NEGATIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift + S16_60_PHASE_SHIFT );
                }
                else
                {
                }

                break;

            case STATE_1:
                if ( bPrevHallState == STATE_5 )
                {
                    pHandle->Direction = POSITIVE;
                    pHandle->MeasuredElAngle = pHandle->PhaseShift + S16_60_PHASE_SHIFT;
                }
                else if ( bPrevHallState == STATE_3 )
                {
                    pHandle->Direction = NEGATIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift + S16_120_PHASE_SHIFT );
                }
                else
                {
                }

                break;

            case STATE_3:
                if ( bPrevHallState == STATE_1 )
                {
                    pHandle->Direction = POSITIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift + S16_120_PHASE_SHIFT );
                }
                else if ( bPrevHallState == STATE_2 )
                {
                    pHandle->Direction = NEGATIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift + S16_120_PHASE_SHIFT +
                                                            S16_60_PHASE_SHIFT );
                }
                else
                {
                }

                break;

            case STATE_2:
                if ( bPrevHallState == STATE_3 )
                {
                    pHandle->Direction = POSITIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift + S16_120_PHASE_SHIFT
                                                            + S16_60_PHASE_SHIFT );
                }
                else if ( bPrevHallState == STATE_6 )
                {
                    pHandle->Direction = NEGATIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift - S16_120_PHASE_SHIFT );
                }
                else
                {
                }

                break;

            case STATE_6:
                if ( bPrevHallState == STATE_2 )
                {
                    pHandle->Direction = POSITIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift - S16_120_PHASE_SHIFT );
                }
                else if ( bPrevHallState == STATE_4 )
                {
                    pHandle->Direction = NEGATIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift - S16_60_PHASE_SHIFT );
                }
                else
                {
                }

                break;

            case STATE_4:
                if ( bPrevHallState == STATE_6 )
                {
                    pHandle->Direction = POSITIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift - S16_60_PHASE_SHIFT );
                }
                else if ( bPrevHallState == STATE_5 )
                {
                    pHandle->Direction = NEGATIVE;
                    pHandle->MeasuredElAngle = ( int16_t )( pHandle->PhaseShift );
                }
                else
                {
                }

                break;

            default:
                /* Bad hall sensor configutarion so update the speed reliability */
                pHandle->SensorIsReliable = false;

                break;
        }


#ifdef HALL_MTPA
        {
            pHandle->_Super.hElAngle = pHandle->MeasuredElAngle;
        }
#endif

        /* Discard first capture */
        if ( pHandle->FirstCapt == 0u )
        {
            pHandle->FirstCapt++;
            HTU_GetCounter();
        }
        else
        {
            /* used to validate the average speed measurement */
            if ( pHandle->BufferFilled < pHandle->SpeedBufferSize )
            {
                pHandle->BufferFilled++;
            }

            /* Store the latest speed acquisition */
            hHighSpeedCapture = HTU_GetWBRValue(HTU_WBR0);
            wCaptBuf = ( uint32_t )hHighSpeedCapture;
            hPrscBuf =  0;

            /* Add the numbers of overflow to the counter */
            wCaptBuf += ( uint32_t )pHandle->OVFCounter * 0x10000uL;

            if ( pHandle->OVFCounter != 0u )
            {
                /* Adjust the capture using prescaler */
                uint16_t hAux;
                hAux = hPrscBuf + 1u;
                wCaptBuf *= hAux;
            }
            else
            {
                /* If prsc preload reduced in last capture, store current register + 1 */
                if ( pHandle->RatioDec ) /* and don't decrease it again */
                {
                    /* Adjust the capture using prescaler */
                    uint16_t hAux;
                    hAux = hPrscBuf + 2u;
                    wCaptBuf *= hAux;

                    pHandle->RatioDec = false;
                }
                else  /* If prescaler was not modified on previous capture */
                {
                    /* Adjust the capture using prescaler */
                    uint16_t hAux = hPrscBuf + 1u;
                    wCaptBuf *= hAux;
                }
            }

#if 0

            /* Store into the buffer */
            /* Null Speed is detected, erase the buffer */
            if ( wCaptBuf > pHandle->MaxPeriod )
            {
                uint8_t bIndex;

                for ( bIndex = 0u; bIndex < pHandle->SpeedBufferSize; bIndex++ )
                {
                    pHandle->SensorSpeed[bIndex]  = 0;
                }

                pHandle->BufferFilled = 0 ;
                pHandle->SpeedFIFOSetIdx = 1;
                pHandle->SpeedFIFOGetIdx = 0;
                /* Indicate new speed acquisitions */
                pHandle->NewSpeedAcquisition = 1;
                pHandle->ElSpeedSum = 0;
            }
            /* Filtering to fast speed... could be a glitch  ? */
            /* the HALL_MAX_PSEUDO_SPEED is temporary in the buffer, and never included in average computation*/
            else
#endif
            if ( wCaptBuf < pHandle->MinPeriod )
            {
                pHandle->CurrentSpeed = HALL_MAX_PSEUDO_SPEED;
                pHandle->NewSpeedAcquisition = 0;
            }
            else
            {
                pHandle->ElSpeedSum -= pHandle->SensorSpeed[pHandle->SpeedFIFOIdx]; /* value we gonna removed from the accumulator */

                if ( wCaptBuf >= pHandle->MaxPeriod )
                {
                    pHandle->SensorSpeed[pHandle->SpeedFIFOIdx]  = 0;
                }
                else
                {
                    pHandle->SensorSpeed[pHandle->SpeedFIFOIdx] = ( int16_t ) ( pHandle->PseudoFreqConv / wCaptBuf );
                    pHandle->SensorSpeed[pHandle->SpeedFIFOIdx] *= pHandle->Direction;
                    pHandle->ElSpeedSum += pHandle->SensorSpeed[pHandle->SpeedFIFOIdx];
                }

                /* Update pointers to speed buffer */
                pHandle->CurrentSpeed = pHandle->SensorSpeed[pHandle->SpeedFIFOIdx];
                pHandle->SpeedFIFOIdx++;

                if ( pHandle->SpeedFIFOIdx == pHandle->SpeedBufferSize )
                {
                    pHandle->SpeedFIFOIdx = 0u;
                }

                /* Indicate new speed acquisitions */
                pHandle->NewSpeedAcquisition = 1;
            }

            /* Reset the number of overflow occurred */
            pHandle->OVFCounter = 0u;
        }
    }

    return MC_NULL;
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