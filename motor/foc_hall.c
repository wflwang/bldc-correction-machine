/**
 * @file foc_hall.c
 * @brief hall correct
 * @param 
 * @param
 * @retval hall_angle 返回霍尔的角度
 */

#include "foc_hall.h"
#include "hk32m07x.h"
#include "utils_math.h"
#include "mc_type.h"
#include "mc_config.h"
#include "hw_correct.h"
#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include "mc_tasks.h"
#include "hw_correct.h"
#include "utils_math.h"

static volatile uint32_t m_ang60_intTime=0;  //60度换向时间单位是 1/8us 最长 512s
static int16_t lastHallEAngle = 0;  //上次hall保存的电角度
//int16_t foc_hall_ang_table[8] = {    //0 7 fail
//    0,0,10922,21845,32767,-21845,-10922,0
//};
static int foc_hall_ang_Temptable[8] = {
    0,0,10922,21845,32767,-21845,-10922,0
};
static void M_HALL_TIMx_CC_IRQHandler( void * pHandleVoid );
static void M_HALL_TIMx_UP_IRQHandler( void * pHandleVoid );

//hall struct define

/**
 * @brief hall IRQ
 * 
 * 
 */
void HTU_IRQHandler(void){
    /* HALL Timer Update IT always enabled, no need to check enable UPDATE state */
	uint16_t flag = HTU->HIFR;
	HTU->HIFR = flag;
    if (((flag & (uint16_t)HTU_IT_OVF) != (uint16_t)RESET)) 
    {
        //HTU->HIFR = flag;
        //hall 计数器溢出中断
        //HTU->HIFR  = (uint16_t)HTU_IT_OVF;
		//HTU->HIFR  = (uint16_t)HTU_IT_SW;
		//HTU->HIFR = HTU_IT_OVF | HTU_IT_SW;
        M_HALL_TIMx_UP_IRQHandler(&HALL_M1);
		//HTU->HIFR = HTU_IT_OVF | HTU_IT_SW;
        //HTU->HIFR  = (uint16_t)HTU_IT_OVF;
    }

    /* HALL Timer CC1 IT always enabled, no need to check enable CC1 state */
    if (((flag & (uint16_t)HTU_IT_SW) != (uint16_t)RESET))  
    {
        //数据变化中断
        //HTU->HIFR = flag;
        //HTU->HIFR  = (uint16_t)HTU_IT_SW;
        M_HALL_TIMx_CC_IRQHandler(&HALL_M1);
        //HTU->HIFR  = (uint16_t)HTU_IT_SW;
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
 * hall 学习时候 检测到hall 值变化就记录当前变化的时间,和电角度对应的hall 第一次不要 剩余6次后 完整记录下hall跳变点
 * 和跳变时间 下次时间由本次时间的2/3 快速掠过 后段1/3再电机慢慢变化角度检测跳变点
 * 
*/
static void M_HALL_TIMx_CC_IRQHandler( void * pHandleVoid )
{
    foc_hall_t * pHandle = ( foc_hall_t * ) pHandleVoid;
    int ang_diff=0;   //两次有效的角度偏差
    int32_t erpm = 0;
    //uint8_t bPrevHallState;
    //uint32_t wCaptBuf;
    //uint16_t hPrscBuf;
    //int16_t nowEAngle= 0;   //当前电角度
	int16_t ang_hall_int=0;
	//int16_t tempL_int_diff=0;
	//int16_t tempR_int_diff=0;
    //int16_t tempL_int = 0; //下次误差预测
    //int16_t tempR_int = 0;
    //uint32_t hHighSpeedCapture;
    //霍尔值
    uint32_t hall_val = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
                        | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
                        | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
    //uint32_t hall_val1 = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
    //                    | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
    //                    | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
    //uint8_t count=0;
    //while((hall_val!=hall_val1)&&(count<4)){
    //    count++;
    //    hall_val = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
    //                    | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
    //                    | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
    //    hall_val1 = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
    //                    | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
    //                    | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
    //}
    //对应出不同角度值 0,1,2,3,4,5,6,7
    //每次进来比较本次和上次的时间误差
    // 角度变化/时间误差 = 当前变化的角速度 *dt(中断时间) = 每次中断预计变化的角度
    // 如果变化的角度可能会超过下一次的位置 但实际没有 就没变变化1/2角度等待新的到来 如果等待次数超过认为hall error
    //如果变化的数据不是正常的数据 霍尔检测错误, 则
    // 用当前的角速度 每次中断估算变化的角度
    // hall_speed 也要做一阶平滑PI控制
    //60度换向时间
    //每次hall变化的时间放在 HTU_WBR0中
	static uint8_t last_hall = 0xFF;
    if( hall_val == last_hall )
    {
        // 没变化！直接退出！不处理！
        return;
    }
    // 变化了才保存，才继续执行
    last_hall = hall_val;
    m_ang60_intTime += HTU_GetWBRValue(HTU_WBR0);  //获取本次误差的时间
    if(m_ang60_intTime>MaxAng60IntTime){    //超过最长60度换向时间 就报错
        //长时间不换相突然检测到换相,说明hall恢复了,检测现在角度能不能用 可以用的话直接输出当前角度,但是插值速度=0 角度保持不变 直到下次hall变化
        //motor->anginc = 0;    //每次变化的角度
        //XorEn();
        m_ang60_intTime = MaxAng60IntTime;  //最长时间限制 换相最长时间
        //motor->m_ang_hall_int_prev = 65536; //给一个错误的角度
    }
    if((hall_val==0)||(hall_val>=7)){
        //如果hall 值是0/7 不应该出现的值 可能是速度太快霍尔检测错误 120度安装
        //此时错误不算时间 也不更新速度,下次计数应该用错误计数的值和下次值的累加和
        return; //不计算速度和时间
    }
    //XorEn();
    //if(hall_val==5)
    //    XorEn();
    pHandle->hall_val = (uint8_t)hall_val; //输出新的角度,主程序中可以知道角度的更新 方便映射电角度
    //nowEAngle = pHandle->real_phase;   //获取当前电机的电角度
    //0 正常模式? 1,2,3,4,5,6,7(学习) 0xff(hall 没有学习过), 0xfe(hall 不存在)
    switch(pHandle->hallState){
        case hall_run: //正常模式
            //120度hall 顺序 1-3-2-6-4-5-1  / 5-4-6-2-3-1-5
            ang_hall_int = pHandle->foc_hall_table[hall_val];	//当前值对应表格位置的角度值
            if(m_ang60_intTime>=MaxAng60IntTime){   //如果上次超时了 认为速度过慢/堵转了 重新开始hall 角度
                //上次是否角度不正常 不正常直接赋值正常角度
                //从误差过来计算角速度时间默认最大
                //motor->angWspeed = MaxAngWSpeed; //第一次默认角速度时间最长? 基本没有补偿
                pHandle->anginc = 0;    //每次变化的角度 超时不插值处理
                pHandle->m_ang60_intTime = 0;   //当前60度时间无效
                m_ang60_intTime = 0;    //重新更新下一次时间
                pHandle->m_ang_hall_int_prev  = ang_hall_int; //更新角度
                pHandle->m_ang_hall_int_Next  = ang_hall_int; //更新角度
				pHandle->angUpdate = true;
            }else if(ang_hall_int != pHandle->m_ang_hall_int_prev){
                //换相在限制时间内
                //角度变化不一样 上次和本次角度都是有效 才可以更新角度,计算出角速度
                //    ang/m_ang60_intTime   角度/时间  = 角速度
                //必须要检测角度变化最大 范围 不可超过最大范围
                ang_diff = (int)ang_hall_int - (int)pHandle->m_ang_hall_int_prev;   //角度变化的插值
                //hall 变化超过一半的角度变化认为是反转 直接减去65536
                //int16-int16 不需要限幅
                if(ang_diff>32767){
                    ang_diff -= 65536;
                }else if(ang_diff<-32768){
                    ang_diff += 65536;
                }
                //预测下次的角度 做最大插值限制
                //switch(hall_val)
                //{
                //    case 1:
                //        tempL_int = pHandle->foc_hall_table[3];
                //        tempR_int = pHandle->foc_hall_table[5];
                //        break;
                //    case 2:
                //        tempL_int = pHandle->foc_hall_table[3];
                //        tempR_int = pHandle->foc_hall_table[6];
                //        break;
                //    case 3:
                //        tempL_int = pHandle->foc_hall_table[1];
                //        tempR_int = pHandle->foc_hall_table[2];
                //        break;
                //    case 4:
                //        tempL_int = pHandle->foc_hall_table[5];
                //        tempR_int = pHandle->foc_hall_table[6];
                //        break;  
                //    case 5:
                //        tempL_int = pHandle->foc_hall_table[1];
                //        tempR_int = pHandle->foc_hall_table[4];
                //        break;
                //    case 6:
                //    default:
                //        tempL_int = pHandle->foc_hall_table[2];
                //        tempR_int = pHandle->foc_hall_table[4];
                //        break;
                //}
                //tempL_int_diff = tempL_int-ang_hall_int; //下次误差预测
                //tempR_int_diff = tempR_int-ang_hall_int;
//
                //if(ang_diff>0){
                //    //角度变大 预测下次角度不能超过右边的值
                //    if(tempL_int_diff>0){ //方向一致 本次角度的下一个角度 也是增加
                //        pHandle->m_ang_hall_int_Next = tempL_int;   //下次插值的终点
                //    }else{ 
                //        pHandle->m_ang_hall_int_Next = tempR_int;   //下次插值的终点
                //    }
                //}else{
                //    //角度变小 预测下次角度不能超过左边的值
                //    if(tempR_int_diff<0){ //方向一致
                //        pHandle->m_ang_hall_int_Next = tempR_int;   //下次插值的终点
                //    }else{ 
                //        pHandle->m_ang_hall_int_Next = tempL_int;   //下次插值的终点
                //    }
                //}
                //误差变化方向是否一致 一致时候计算变化时间才有效 否则用上次60度时间
                //每次中断中的hall角度用算出的 变化的角度/60度时间 * 中断时间 = 每次变化的角度
                int16_t abs_ang_diff = ang_diff;
                if(abs_ang_diff<0)
                    abs_ang_diff = -abs_ang_diff;
                //int16_t abs_ang_diff = abs(ang_diff);
                if((DirCMPint16((int16_t)ang_diff,pHandle->last_ang_diff)==0)||(pHandle->last_ang_diff==0)){   //误差变化方向是否一致 
                    //误差方向一致
                    //XorEn();
                    if(abs_ang_diff>hEdegree(80)){    //>80度 认为只变化80度
                        //本次角度变化过大 限制更新角度 本次误差如果>80度 可能是上次60度变化丢失了 duty限制要降下来
                        pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev + (int16_t)SignCMPint16(ang_diff,hEdegree(40)); 
                    }else if(abs_ang_diff<hEdegree(40)){  //<40度 直接认为变化60度吗？
                        pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev + (int16_t)SignCMPint16(ang_diff,hEdegree(30)); 
                    }else{
                        pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev + (int16_t)(ang_diff>>1); //正常更新角度 增加上次hall角度的一般到跳变点
                    }
                    pHandle->m_ang60_intTime = m_ang60_intTime; //更新时间
                    m_ang60_intTime = 0;    //重新更新下一次时间
                    pHandle->last_ang_diff = ang_diff;    //更新误差 主要是更新误差方向
                    //中断每次增加的角度 *中断时间(单位1/64us) = 每次增加的角度 更新插值大小
                    //计算当前速度下 中断需要补偿的角度
                    if(pHandle->m_ang60_intTime==0)
                        pHandle->m_ang60_intTime = 1;
                    int32_t tmp = pHandle->last_ang_diff * (int32_t)pHandle->intTime * Kpdiff;
                    //变化的角度 * 中断时间(1/64us) / 变化的时间(1/64us) = 每次中断变化的角度;
                    //扩大16倍提高精度
                    pHandle->anginc = (tmp<<4) / pHandle->m_ang60_intTime;  //每次中断变化的角度 
                    erpm = (int32_t)ScaleErpm*(int)pHandle->last_ang_diff / pHandle->m_ang60_intTime;
                    pHandle->erpm = UTILS_LPInt32_FAST(pHandle->erpm,erpm,(int32_t)(0.75*32767)); //获取本次电角速度
                    pHandle->m_ang_hall_int_prev = ang_hall_int;
                    pHandle->angUpdate = true;   //中断允许更新最新角度了 中断中清除
                    //if(ang_diff>0){
                    //    //误差变大
                    //    motor->m_ang60_intTime = m_ang60_intTime;   //60度换相时间  这个时间可直到hall的速度 时间/变化的电角度 等于角速度
                    //}else{
                    //    //误差变小 时间增量为-
                    //    motor->m_ang60_intTime = -m_ang60_intTime;
                    //}
                }else{
                    //本次误差变化方向和上次误差变化方向不一致 用上次误差产生的时间？ 时间不能突变的特性
                    pHandle->last_ang_diff = -pHandle->last_ang_diff;    //更新误差 主要是更新误差方向 下次 方向就可以对了
                    //方向可以反 但不能跳60度  不允许60度 只能用上次的初始角度  motor->m_ang_hall_int_prev 还得用上次的
                    //用上次触发中断的时间 反向
                    pHandle->anginc = -pHandle->anginc; //插值方向反一下,但是插值速度不变 继续用上次的速度插值 直到下次hall变化
                    //方向不一致时候时间用上次的速度也用上次的?
                    //motor->m_ang60_intTime = -motor->m_ang60_intTime; //如果角度不同的话用上次,防止正反转误差  
                    //用上次的角度
                    if(abs_ang_diff>hEdegree(80)){    //>80度 认为只变化80度
                        //本次角度变化过大 限制更新角度 本次误差如果>80度 可能是上次60度变化丢失了 duty限制要降下来
                        pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev + (int16_t)SignCMPint16(ang_diff,hEdegree(40)); 
                    }else if(abs_ang_diff<hEdegree(40)){  //<40度 直接认为变化60度吗？
                        pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev + (int16_t)SignCMPint16(ang_diff,hEdegree(30)); 
                    }else{
                        pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev + (int16_t)(ang_diff>>1); //正常更新角度 增加上次hall角度的一般到跳变点
                    }
                    //pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev;
                    //pHandle->m_ang_hall_int_prev  = ang_hall_int; //本次角度直接更新 下次以这个角度开始算误差
                    //预测下次的角度 做最大插值限制
                    pHandle->erpm = 0;
                    //pHandle->erpm = -pHandle->erpm;
                    //pHandle->erpm = UTILS_LPInt32_FAST(pHandle->erpm,(-pHandle->erpm),(int32_t)(0.3*32767)); //获取本次电角速度          
                    pHandle->m_ang_hall_int_prev = ang_hall_int;
                    pHandle->angUpdate = true;   //中断允许更新最新角度了
                }
            }
        break;
        case hall_null:  //hall 没有学习过
            m_ang60_intTime = 0;
        break;
        case hall_learnOver:    //学习结束
            m_ang60_intTime = 0;
        break;
        case hall_learnStart: //学习后第一次触发到
            lastHallEAngle = pHandle->real_phase;   //获取当前电机的电角度
            pHandle->hallState++;   // next step
            //memset(foc_hall_ang_Temptable, 0, sizeof(foc_hall_ang_Temptable));
			foc_hall_ang_Temptable[1] = 0;
			foc_hall_ang_Temptable[2] = 0;
			foc_hall_ang_Temptable[3] = 0;
			foc_hall_ang_Temptable[4] = 0;
			foc_hall_ang_Temptable[5] = 0;
			foc_hall_ang_Temptable[6] = 0;
            m_ang60_intTime = 0;
        break;
        //正转2圈 反转2圈 共4次取平均
        default:    //第二次触发到开始记录 记录当前角度和相对上次变化的角度 推测下一个可能的角度
            if(pHandle->hallState&0x80){
                //上次hall 失效 这次hall 恢复了 记录当前角度 但是不进行角度插值 初始化到当前角度
                pHandle->anginc = 0;    //每次变化的角度
                //hall 失效/丢失 直接退出 不记录数据
                //return;
                break;
            }
            m_ang60_intTime = 0;
			//return;
            //2,3,4,5,6,7, 8,9,10,11,12,13,  14(12),15(11),16(10),17(9),18(9),19(7),  20(6),21(5),22(4),23(3),24(2),25(13)  正反两圈
            //计算电角度的变化
            ang_diff = (int)pHandle->real_phase - (int)lastHallEAngle; //上次角度 - 本次角度 = 角度的变化值
			if (ang_diff > 32767)
				ang_diff -= 65536;
			else if (ang_diff < -32768)
				ang_diff += 65536;
            //if(ang_diff>32767){
            //    ang_diff -= 65536
            //}else if(ang_diff<-32768){
            //    ang_diff += 65536
            //}
            //是下次hall 可以快速跳过的角度 下次能快速到达的角度
            pHandle->hallFastLearnAngDiff = (int16_t)(((int32_t)ang_diff*nextPro)>>8);    //下次快速更新到的角度 省去78%的慢速时间 剩余<3s 1ms变化一次
            //记住当前角度
            foc_hall_ang_Temptable[hall_val] = foc_hall_ang_Temptable[hall_val]+(int)pHandle->real_phase;    //赋值当前电角度
            lastHallEAngle = pHandle->real_phase;
            pHandle->hallState++;
			//3(+1),4,5,6,7,8(+6)  9,10,11,12,13,14 --- 15 16 17 18 19 20, 21 22 23 24 25 26
			//return;
            if(HALL_M1.hallState==((hallLearnEnd/2)+1))
                pHandle->hallFastLearnAngDiff = 0;
            if(pHandle->hallState>=hallLearnEnd){
                //4轮了 每个点有4轮角度数据 取平均
                for(uint8_t i=0;i<8;i++){
                    pHandle->foc_hall_table[i] = (int16_t)(foc_hall_ang_Temptable[i]>>2);    //逐个更新hall table
                }
                //结束hall学习  正反转后学习的位置是 hall的中点角度位置
                pHandle->m_ang_hall_int_prev = pHandle->real_phase; //给最后校准的角度
                pHandle->m_ang_hall_int_Next = pHandle->real_phase;
                pHandle->m_ang60_intTime = 0;   //最长换相时间 后续基本插值无效
                pHandle->anginc = 0;    //每次变化的角度  不插值处理
                //clearRefIdq();  //清除iqd 电流
                pHandle->angUpdate = true; 
                //pHandle->hallState = hall_run; //进入正常模式
                pHandle->hallState = hall_learnOver; //学习结束
            }
        break;
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
 * 霍尔计数器溢出了
*/
static void M_HALL_TIMx_UP_IRQHandler( void * pHandleVoid )
{
    foc_hall_t * pHandle = ( foc_hall_t * ) pHandleVoid;
    m_ang60_intTime += 0x1000000;  //+24bit 溢出的时间  262,144us
    if(m_ang60_intTime>MaxAng60IntTime){    //超过最长60度换向时间 就报错
        //换相超时
        m_ang60_intTime = MaxAng60IntTime;
        //锁定当前角度 慢慢向霍尔读取的速度靠拢 如果读取的角度正常 不正常就锁定当前速度
        //motor->m_ang_hall_int_prev = 65536; //给一个错误的角度
        uint32_t hall_val = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
                            | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
                            | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
        //uint32_t hall_val1 = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
        //                    | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
        //                    | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
        //uint8_t count=0;
        //while((hall_val!=hall_val1)&&(count<3)){
        //    count++;
        //    hall_val = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
        //                    | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
        //                    | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
        //    hall_val1 = (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO1, HW_HALL_ENC_PIN1 ) << 2)
        //                    | (GPIO_IsInputPinSet( HW_HALL_ENC_GPIO2, HW_HALL_ENC_PIN2 ) << 1)
        //                    | GPIO_IsInputPinSet( HW_HALL_ENC_GPIO3, HW_HALL_ENC_PIN3 );
        //}
        if((hall_val==0)||(hall_val>=7)){
			//长时间不换相 且角度不合法 说明hall坏了 角度不动 速度慢慢降到0 且不能再启动 直到hall恢复正常
			pHandle->hallState |= 0x80;    //hall 失效/丢失
			pHandle->anginc = 0;    //每次变化的角度
            //return; //不计算速度和时间
        }else{
            if(pHandle->hallState==hall_run){
                pHandle->erpm = 0;
                //pHandle->erpm = UTILS_LPInt32_FAST(pHandle->erpm,0,(int32_t)(0.6*32767)); //获取本次电角速度
                //角度合法 说明电机堵转了 没有动 此时目标角度设置为当前hall角度 当前值慢慢靠近这个角度
                pHandle->anginc = 0;    //每次变化的角度0 长时间不换相 认为电机堵转了 角度不动 速度慢慢降到0 且不能再启动 直到hall恢复正常
                pHandle->m_ang_hall_int_prev = pHandle->foc_hall_table[hall_val];
                pHandle->m_ang_hall_int_Next = pHandle->m_ang_hall_int_prev;
                HALL_M1.angUpdate = true;
            }
        }
    }
}
/**
 * @brief hall init config 
 * 
 */
void M_Hall_Init(foc_hall_t * pHandle,mc_config_t *mcconf){
    m_ang60_intTime = 0;	//MaxAng60IntTime;  //60度最长换相时间
    pHandle->foc_hall_table = mcconf->foc_hall_table;
    //pHandle->foc_hall_tableTemp = foc_hall_ang_Temptable;
    pHandle->anginc  = 0;
    //motor->m_ang_hall_int_prev = 65536; //给一个错误的角度
    /* Clear the TIMx's pending flags */
    HTU->HIFR = 0xFFFF;
    /* Source of Update event is only counter overflow/underflow */
    //HTU_ITConfig(HTU_IT_SW | HTU_IT_ERR | HTU_IT_OVF, ENABLE);
	HTU_ITConfig(HTU_IT_SW | HTU_IT_OVF, ENABLE);
    /* Enable PWM output and Start Counter */
    HTU_CounterCmd(ENABLE);
}

/**
 * @brief 获取上次hall学习的角度误差
 * 
 * 
*/
int16_t GetLastLearnAngDiff(foc_hall_t * pHandle){
    int ang_diff = (int)pHandle->real_phase - (int)lastHallEAngle; //上次角度 - 本次角度 = 角度的变化值
    if (ang_diff > 32767)
		ang_diff -= 65536;
	else if (ang_diff < -32768)
		ang_diff += 65536;
    return (int16_t)ang_diff;
}
/**
 * @brief check hall table right?
 * 
 * hall -> 1 3 2 6 4 5
 * 
 */
bool CheckHallTab(foc_hall_t * pHandle){
    int16_t diff = pHandle->foc_hall_table[1]-pHandle->foc_hall_table[3];
    if(diff<0)
        diff = -diff;
    // >50度 <70度
    if((diff<9102)||(diff>12743))
        return false;   // 角度校准失败
    diff = pHandle->foc_hall_table[3]-pHandle->foc_hall_table[2];
    if(diff<0)
        diff = -diff;
    // >50度 <70度
    if((diff<9102)||(diff>12743))
        return false;   // 角度校准失败
    diff = pHandle->foc_hall_table[2]-pHandle->foc_hall_table[6];
    if(diff<0)
        diff = -diff;
    // >50度 <70度
    if((diff<9102)||(diff>12743))
        return false;   // 角度校准失败
    diff = pHandle->foc_hall_table[6]-pHandle->foc_hall_table[4];
    if(diff<0)
        diff = -diff;
    // >50度 <70度
    if((diff<9102)||(diff>12743))
        return false;   // 角度校准失败
    diff = pHandle->foc_hall_table[4]-pHandle->foc_hall_table[5];
    if(diff<0)
        diff = -diff;
    // >50度 <70度
    if((diff<9102)||(diff>12743))
        return false;   // 角度校准失败
    diff = pHandle->foc_hall_table[5]-pHandle->foc_hall_table[1];
    if(diff<0)
        diff = -diff;
    // >50度 <70度
    if((diff<9102)||(diff>12743))
        return false;   // 角度校准失败
    return true;
}
/**
 * @brief correct hall
 * 
 * @retval hall_angle
 * 在ADC中断中计算hall的插值补偿角度
 */
//int16_t foc_correct_hall(void){
//}
