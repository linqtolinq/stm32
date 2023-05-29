#ifndef _KEY_H
#define _KEY_H

#include "main.h"
#define key_clk_en 	 __HAL_RCC_GPIOC_CLK_ENABLE()
#define keyport GPIOC
#define keypin  GPIO_PIN_13
void key_gpio_nint(void);
#endif
