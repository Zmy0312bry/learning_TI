/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// #include "stepdriver/stepper.h"
#include "stepdriver/gimbal.h"
#include "ti_msp_dl_config.h"
#include <ti/driverlib/m0p/dl_interrupt.h>

#include "stdio.h"

/* This is the space for you to set general varible */
int pulse_number_up = 0;
int pulse_number_down = 0;
volatile unsigned char uart_data = 0;
uint32_t steps_up;
uint32_t stepper_state_up;

uint32_t steps_down;
uint32_t stepper_state_down;
/* This is the end of general varible */

/* This plaace is for you to declare functions */
void uart0_send_char(char ch); //串口0发送单个字符
/* This is the end of function */

int main(void) {
  SYSCFG_DL_init();

  /* This is the init function */
  NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
  // 清除串口中断标志
  NVIC_ClearPendingIRQ(UART_TTL_INST_INT_IRQN);
  // 使能串口中断
  NVIC_EnableIRQ(UART_TTL_INST_INT_IRQN);
  /* This is the end of init */
  // STEP_MOTOR_setAngle(-90.0f, GPIO_DIR_PORT, GPIO_DIR_PIN_0_PIN,PWM_Stepper_INST, GPIO_PWM_Stepper_C0_IDX);
  GIMBAL_setAngle(90,90,&steps_up,&steps_down,&stepper_state_up,&stepper_state_down);
  while (1) {
    // STEP_MOTOR_UpDate(PWM_Stepper_INST, GPIO_PWM_Stepper_C0_IDX);
    GIMBAL_Update(&stepper_state_up,&stepper_state_down);
  }
}

/* This is the place for IRQHandlers */
void GROUP1_IRQHandler(void) // Group1的中断服务函数
{
  // 读取Group1的中断寄存器并清除中断标志位
  switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
  // 检查是否是KEY的GPIOB端口中断
  case PWM_Counter_Up_PIN_1_IIDX:
    if (pulse_number_up >= steps_up) {
      stepper_state_up = MOTOR_STOP;
      pulse_number_up = 0;
    } else {
      pulse_number_up++;
    }
    break;
  case PWM_Counter_Down_PIN_3_IIDX:
    if (pulse_number_up >= steps_down) {
      stepper_state_down = MOTOR_STOP;
      pulse_number_up = 0;
    } else {
      pulse_number_up++;
    }
  }
}

void UART_TTL_INST_IRQHandler(void) {
  // 如果产生了串口中断
  switch (DL_UART_getPendingInterrupt(UART_TTL_INST)) {
  case DL_UART_IIDX_RX: // 如果是接收中断
    // 将发送过来的数据保存在变量中
    uart_data = DL_UART_Main_receiveData(UART_TTL_INST);
    // 将保存的数据再发送出去
    uart0_send_char(uart_data);
    break;

  default: // 其他的串口中断
    break;
  }
}

/* This is the end */

//串口发送单个字符
void uart0_send_char(char ch)
{
    //当串口0忙的时候等待，不忙的时候再发送传进来的字符
    while( DL_UART_isBusy(UART_TTL_INST) == true );
    //发送单个字符
    DL_UART_Main_transmitData(UART_TTL_INST, ch);
}

// 重定向fputc函数
int fputc(int ch, FILE *stream) {
  while (DL_UART_isBusy(UART_TTL_INST) == true)
    ;
  DL_UART_Main_transmitData(UART_TTL_INST, ch);
  return ch;
}

// 重定向fputs函数
int fputs(const char *restrict s, FILE *restrict stream) {

  uint16_t char_len = 0;
  while (*s != 0) {
    while (DL_UART_isBusy(UART_TTL_INST) == true)
      ;
    DL_UART_Main_transmitData(UART_TTL_INST, *s++);
    char_len++;
  }
  return char_len;
}
int puts(const char *_ptr) { return 0; }