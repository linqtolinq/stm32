#ifndef _DAC_TASK_H
#define _DAC_TASK_H

#include "main.h"
#define MAX_DAC_BUFFER_SIZE 50
typedef struct _DAC_Handle
{
    // uint16_t Dac_Buffer[MAX_DAC_BUFFER_SIZE];
    uint16_t Dac_S_T_Buffer[MAX_DAC_BUFFER_SIZE];
    TIM_HandleTypeDef* DAC_Handle_Clock;
}DAC_Handle;
extern DAC_Handle DAC_Handler;
void DAC_Init(void);
#endif
