#include "dac_task.h"
#include "tim.h"
#include "APP.h"

DAC_Handle DAC_Handler;
void DAC_Init(void)
{
    DAC_Handler.DAC_Handle_Clock = &htim4;
}
