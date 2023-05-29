#include "Key.h"


void key_gpio_nint(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    key_clk_en;
    GPIO_InitStruct.Pin = keypin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(keyport, &GPIO_InitStruct);
}

