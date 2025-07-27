// stepper.h
#ifndef STEPPER_H
#define STEPPER_H

#include "ti_msp_dl_config.h"
#include <math.h>
// 用户可配置的宏
#ifndef SPLIT_NUMBER
#define SPLIT_NUMBER 16 // 默认16细分
#endif

#ifndef STEP_ANGLE
#define STEP_ANGLE 1.8f // 默认步距角1.8度
#endif

#ifndef PULSE_WIDTH_CYCLES
#define PULSE_WIDTH_CYCLES 32 * 100000 // 默认脉冲宽度1毫秒 (32MHz下)
#endif

#ifndef DIR_STABLE_CYCLES
#define DIR_STABLE_CYCLES 32 * 1000 // 方向稳定时间1微秒 (32MHz下)
#endif


// 定义电机状态
#define    MOTOR_MOVING 1      // 运动状态
#define    MOTOR_STOP 2       // 停止状态


// 设置电机旋转角度
void STEP_MOTOR_setAngle(float angle, 
                         GPIO_Regs *dir_gpio, uint32_t dir_pin,
                         GPTIMER_Regs *step_inst, DL_TIMER_CC_INDEX step_idx,
                         uint32_t* step_num, uint32_t* motor_state);

void STEP_MOTOR_UpDate(GPTIMER_Regs *step_inst, DL_TIMER_CC_INDEX step_idx, uint32_t* motor_state);


#endif // STEPPER_H