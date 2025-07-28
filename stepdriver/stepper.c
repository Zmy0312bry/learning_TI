// stepper.c
#include "stepper.h"


void STEP_MOTOR_setAngle(float angle, 
                         GPIO_Regs *dir_gpio, uint32_t dir_pin,
                         GPTIMER_Regs *step_inst, DL_TIMER_CC_INDEX step_idx, 
                         uint32_t* step_num, uint32_t* motor_state)
{
    // 计算需要的步数 (步数 = |角度| * 细分度 / 步距角)
    float steps_f = fabsf(angle) * SPLIT_NUMBER / STEP_ANGLE;
    *step_num = (uint32_t)(steps_f + 0.5f);  // 四舍五入
    
    if(step_num == 0) return;  // 不需要移动
    
    // 设置方向（正角度正转，负角度反转）
    if(angle >= 0) {
        DL_GPIO_setPins(dir_gpio, dir_pin);  // 正转（高电平）
    } else {
        DL_GPIO_clearPins(dir_gpio, dir_pin); // 反转（低电平）
    }
    
    // 方向稳定时间（确保方向信号稳定）
    delay_cycles(DIR_STABLE_CYCLES);
    
    // 生成指定脉冲PWM
    DL_TimerG_setCaptureCompareValue(step_inst,1000,step_idx);
    *motor_state=MOTOR_MOVING;

}

void STEP_MOTOR_UpDate(GPTIMER_Regs *step_inst, DL_TIMER_CC_INDEX step_idx,uint32_t* motor_state){
    if (*motor_state==MOTOR_STOP) {
        DL_TimerG_setCaptureCompareValue(step_inst,0,step_idx);
    }
    else return;
}