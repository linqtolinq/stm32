#ifndef _LIGHT_LED_H
#define _LIGHT_LED_H

#include "main.h"

#define LED1_ON HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,1)
#define LED1_OFF HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,0)
#define LED1_TOGGLE HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin)

#endif