/**
 * @file foc_hall.h
 * @author MaxwellWang
 * 
 * 
 */

 #ifndef FOC_HALL_H_
 #define FOC_HALL_H_

 #include   "datatypes.h"
 #include   "hk32m07x_gpio.h"

// Hall sensor constants
#define MaxAng60IntTime 0xFFFFFF // Maximum 60-degree commutation time
#define MaxAngWSpeed 0xFFFFFF    // Maximum angular speed

// Hall sensor handler structure
typedef struct {
    GPIO_TypeDef* H1Port;
    uint16_t H1Pin;
    GPIO_TypeDef* H2Port;
    uint16_t H2Pin;
    GPIO_TypeDef* H3Port;
    uint16_t H3Pin;
} HALL_Handle_t;

// External variables
extern HALL_Handle_t HALL_M1;

// Function declarations
void HTU_IRQHandler(void);
void * HALL_TIMx_CC_IRQHandler( void * pHandleVoid );
void * HALL_TIMx_UP_IRQHandler( void * pHandleVoid );
int16_t foc_correct_hall_interrupt(void);

 #endif