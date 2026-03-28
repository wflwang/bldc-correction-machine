/**
  ******************************************************************************
  * @file    drive_parameters.h
  * @version V1.0.0
  * @brief   This file contains the parameters for MOTOR controler.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVE_PARAMETERS_H
#define __DRIVE_PARAMETERS_H

/************************
 *** Motor Parameters ***
 ************************/

/************************* CPU & ADC PERIPHERAL CLOCK CONFIG ******************/
#define SYSCLK_FREQ                          64000000uL
#define TIM_CLOCK_DIVIDER                    1
#define ADV_TIM_CLK_MHz                      64
#define ADC_CLK_MHz                          16uL /* Maximum ADC Clock Frequency expressed in MHz */
#define HALL_TIM_CLK                         64000000uL
#define ADC_REFERENCE_VOLTAGE                5
/***************** MOTOR ELECTRICAL PARAMETERS  ******hDTCompCnt************************/
#define POLE_PAIR_NUM                        4       /*!< Number of motor pole pairs */
#define RS                                   3.2     /*!< Stator resistance, ohm */
#define LS                                   0.00043 /*!< Stator inductance, H For I-PMSM it is equal to Lq */

/* When using Id = 0, NOMINAL_CURRENT is utilized to saturate the output of the
   PID for speed regulation (i.e. reference torque).
   Transformation of real currents (A) into int16_t format must be done accordingly with
   formula:
   Phase current (int16_t 0-to-peak) = (Phase current (A 0-to-peak)* 32767 * Rshunt *
                                   *Amplifying network gain)/(MCU supply voltage/2)
*/
#define NOMINAL_CURRENT                      2100
#define MOTOR_MAX_SPEED_RPM                  4000 /*!< Maximum rated speed  */
#define MOTOR_VOLTAGE_CONSTANT               2.45 /*!< Volts RMS ph-ph /kRPM */

/***************** MOTOR SENSORS PARAMETERS  ******************************/
/* Motor sensors parameters are always generated but really meaningful only
   if the corresponding sensor is actually present in the motor         */

/*** Hall sensors ***/
#define HALL_SENSORS_PLACEMENT               DEGREES_120   /*!< Define here the mechanical position of the sensors withreference to an electrical cycle.It can be either DEGREES_120 or DEGREES_60 */
#define HALL_PHASE_SHIFT                     120           /*!< Define here in degrees the electrical phase shift between the low to high transition of signal H1 and the maximum of the Bemf induced on phase A */

/*********** Bus voltage sensing section ****************/
#define VBUS_PARTITIONING_FACTOR             0.091         /*!< It expresses how much the Vbus is attenuated before being converted into digital value */
#define NOMINAL_BUS_VOLTAGE_V                24

/******** Current reading parameters section ******/
/*** Topology ***/
#define THREE_SHUNT
#define RSHUNT                               0.05

/*  ICSs gains in case of isolated current sensors, amplification gain for shunts based sensing */
#define AMPLIFICATION_GAIN                   3.2//5.18 
#define AMPLIFICATION_GAIN_IBUS              3.2

/*** Noise parameters ***/
#define TNOISE_NS                            1000
#define TRISE_NS                             1000

/************ Temperature sensing section ***************/
/* V[V]=V0+dV/dT[V/Celsius]*(T-T0)[Celsius]*/
#define V0_V                                 1.055   /*!< in Volts */
#define T0_C                                 25      /*!< in Celsius degrees */
#define dV_dT                                0.023   /*!< V/Celsius degrees */
#define T_MAX                                110     /*!< Sensor measured temperature at maximum power stage working temperature, Celsius degrees */

/******** MAIN AND AUXILIARY SPEED/POSITION SENSOR(S) SETTINGS SECTION ********/

/*** Speed measurement settings ***/
#define MAX_APPLICATION_SPEED                4000 /*!< rpm, mechanical */
#define MIN_APPLICATION_SPEED                0    /*!< rpm, mechanical, absolute value */
#define MEAS_ERRORS_BEFORE_FAULTS            3    /*!< Number of speed  
                                                       measurement errors before 
                                                       main sensor goes in fault */

/****** Hall sensors ************/
#define HALL_MEAS_ERRORS_BEFORE_FAULTS       3 /*!< Number of failed   
                                                    derived class specific speed 
                                                    measurements before main sensor  
                                                    goes in fault */
                                             
#define HALL_AVERAGING_FIFO_DEPTH            16 /*!< depth of the FIFO used to 
                                                     average mechanical speed in 
                                                     0.1Hz resolution */
                                                           
/**************************    DRIVE SETTINGS SECTION   **********************/
/* PWM generation and current reading */
#define PWM_FREQUENCY                        12000
                                             
#define LOW_SIDE_SIGNALS_ENABLING            LS_PWM_TIMER
#define SW_DEADTIME_NS                       1000 /*!< Dead-time to be inserted  
                                                       by FW, only if low side 
                                                       signals are enabled */

/* Torque and flux regulation loops */
#define REGULATION_EXECUTION_RATE            1  /*!< FOC execution rate in number of PWM cycles */

/* Gains values for torque and flux control loops */
#define PID_TORQUE_KP_DEFAULT                1204
#define PID_TORQUE_KI_DEFAULT                242
                                             
#define PID_FLUX_KP_DEFAULT                  1804
#define PID_FLUX_KI_DEFAULT                  542

/* Torque/Flux control loop gains dividers*/
#define TF_KPDIV                             16384
#define TF_KIDIV                             16384

/* Speed control loop */
#define SPEED_LOOP_FREQUENCY_HZ              1000   /*!< Execution rate of speed regulation loop (Hz) */
#define PID_SPEED_KP_DEFAULT                 2212
#define PID_SPEED_KI_DEFAULT                 451
#define PID_SPEED_KD_DEFAULT                 0
                                             
/* Speed PID parameter dividers */           
#define SP_KPDIV                             128
#define SP_KIDIV                             16384

/* Default settings */
#define DEFAULT_CONTROL_MODE                 STC_SPEED_MODE /*!< STC_TORQUE_MODE or STC_SPEED_MODE */
#define DEFAULT_TARGET_SPEED_RPM             1000
#define DEFAULT_TORQUE_COMPONENT             4266
#define DEFAULT_FLUX_COMPONENT               0

/**************************    FIRMWARE PROTECTIONS SECTION   *****************/
#define OV_VOLTAGE_PROT_ENABLING             ENABLE
#define UV_VOLTAGE_PROT_ENABLING             ENABLE
#define OV_VOLTAGE_THRESHOLD_V               50   /*!< Over-voltage threshold */
#define UD_VOLTAGE_THRESHOLD_V               8    /*!< Under-voltage threshold */
#define R_BRAKE_SWITCH_OFF_THRES_V           
#define OV_TEMPERATURE_THRESHOLD_C           110  /*!< Celsius degrees */
#define OV_TEMPERATURE_HYSTERESIS_C          10   /*!< Celsius degrees */

/*  Feed-forward parameters */
/*  Maximum Torque Per Ampere strategy parameters */
#define IQMAX                                15594

#endif /*__DRIVE_PARAMETERS_H*/
