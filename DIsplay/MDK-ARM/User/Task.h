#ifndef __TASK_H
#define __TASK_H

#include "main.h"

#define CH_LEN 295
void Task_Tick(void);
void ADC_Start(void);
extern uint16_t adc_Flag;
extern uint16_t adc_completed_Flag;

extern uint16_t led_Flag;
extern uint32_t adc_buffer_cnt;
extern uint16_t CH_new[CH_LEN*2];
#endif
