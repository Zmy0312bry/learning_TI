#include "gimbal.h"


void GIMBAL_setAngle(float angle_down, float angle_up, uint32_t* steps_up, uint32_t* steps_down,uint32_t* motor_state_up, uint32_t* motor_state_down) {
  // motor down
  STEP_MOTOR_setAngle(angle_down, CHANNEL_DOWN_DIR_REG, CHANNEL_DOWN_DIR_PIN,
                      CHANNEL_DOWN_STEP_REG, CHANNEL_DOWN_STEP_INDEX,steps_down,motor_state_down);

  // motor up
  STEP_MOTOR_setAngle(angle_up, CHANNEL_UP_DIR_REG, CHANNEL_UP_DIR_PIN,
                      CHANNEL_UP_STEP_REG, CHANNEL_UP_STEP_INDEX,steps_up,motor_state_up);
}

void GIMBAL_Update(uint32_t* motor_state_up, uint32_t* motor_state_down) {
  // Motor Down
  STEP_MOTOR_UpDate(CHANNEL_DOWN_STEP_REG, CHANNEL_DOWN_STEP_INDEX,motor_state_down);
  // Motor Up
  STEP_MOTOR_UpDate(CHANNEL_UP_STEP_REG, CHANNEL_UP_STEP_INDEX,motor_state_up);
}