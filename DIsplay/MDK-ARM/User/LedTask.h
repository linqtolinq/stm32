#ifndef __LED_TASK_H
#define __LED_TASK_H
#include "./lcd/bsp_ili9341_lcd.h"

#include "main.h"
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

void Data_Progress(void);

void Model_Transmit (void);
void Drew_sin_wave_Tran(void);
void data_show(void);
#endif

