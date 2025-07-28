#ifndef GIMBAL_H
#define GIMBAL_H
#include "ti_msp_dl_config.h"
#include "stepper.h"

// 上步进
#define CHANNEL_DOWN_DIR_REG     GPIO_DIR_Up_PORT
#define CHANNEL_DOWN_DIR_PIN     GPIO_DIR_Up_PIN_2_PIN
#define CHANNEL_DOWN_STEP_REG    PWM_Stepper_Up_INST
#define CHANNEL_DOWN_STEP_INDEX  GPIO_PWM_Stepper_Up_C0_IDX

// 下步进
#define CHANNEL_UP_DIR_REG     GPIO_DIR_Down_PORT
#define CHANNEL_UP_DIR_PIN     GPIO_DIR_Down_PIN_0_PIN 
#define CHANNEL_UP_STEP_REG    PWM_Stepper_Down_INST
#define CHANNEL_UP_STEP_INDEX  GPIO_PWM_Stepper_Down_C1_IDX  

void GIMBAL_setAngle(float angle_down,float angle_up,
                    uint32_t* steps_up, uint32_t* steps_down,
                    uint32_t* motor_state_up, uint32_t* motor_state_down);

void GIMBAL_Update(uint32_t* motor_state_up, uint32_t* motor_state_down);
#endif