#include "Task.h"
#include "adc.h"
#include "stdio.h"
#include "key.h"
#include "ledtask.h"

uint32_t sysTick = 0;
uint32_t adc_buffer_cnt = 0;
// get data completed
uint16_t adc_Flag = 0;
uint16_t adc_completed_Flag = 0;


uint16_t led_Flag = 0;
uint16_t keydelay = 0;
void Task_Tick(void)
{
    sysTick++;
    sysTick %= 5000; // 5s

    if (sysTick % 200 == 0)
    {
        adc_Flag = 1;
    }

    // key task
    if (HAL_GPIO_ReadPin(keyport, keypin) == 1)
    {
        keydelay++;
        if (keydelay >= 5)
        {
            while (HAL_GPIO_ReadPin(keyport, keypin) == 1);
            Model_Transmit();
            keydelay = 0;
        }
    }
    else
    {
        keydelay = 0;
    }
}

uint16_t CH_new[CH_LEN * 2] = {0};


// uint8_t CH1_Fresh_Start = 0;
// uint8_t CH2_Fresh_Start = 0;

// uint16_t CH1_Counts = 0;
// uint16_t CH2_Counts = 0;

__weak void AD7606_DmaCplCb(DMA_HandleTypeDef *hdma)
{
    printf("0000\r\n");
}

__weak void AD7606_DmaHalfCplCb(DMA_HandleTypeDef *hdma)
{
}

void ADC_Start(void)
{
    
    // HAL_DMA_RegisterCallback(&hdma_adc1, HAL_DMA_XFER_CPLT_CB_ID, AD7606_DmaCplCb);
    // HAL_DMA_RegisterCallback(&hdma_adc1, HAL_DMA_XFER_HALFCPLT_CB_ID, AD7606_DmaHalfCplCb);
    // HAL_DMA_Start_IT(&hdma_adc1, (uint32_t)&hadc1.Instance->DR,(uint32_t)CH_new, CH_LEN * 2);
    // HAL_ADC_Start_DMA(&hadc1,(uint32_t *)CH_new, CH_LEN*2);

    // 启动多通道DMA传输
    // HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)CH_new, 2);

    // HAL_ADCEx_Calibration_Start(&hadc1);
	
	
	  HAL_ADC_Start_IT(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&CH_new, 2 * CH_LEN);
	
	
    adc_buffer_cnt++;
    if (adc_buffer_cnt >= CH_LEN)
    {
        adc_completed_Flag = 1;
    }
    adc_buffer_cnt %= CH_LEN;
}

// DMA传输完成中断回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    adc_completed_Flag = 1;

    HAL_ADC_Stop_IT(&hadc1);
    HAL_ADC_Stop_DMA(&hadc1);
}
