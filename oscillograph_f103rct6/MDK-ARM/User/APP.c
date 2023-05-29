#include "APP.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "stdio.h"
#include "math.h"
#include <string.h>

APP_Demo APP;
void Window_Resresh_Handler(void)
{
    static uint8_t us[8] = {0};
    static uint32_t us_ = 0;
    static uint8_t temp_ = 0;
    static uint8_t temp__ = 0;
    static uint8_t temp_1 = 0;
    static uint8_t temp_2 = 0;
    static uint8_t temp_3[10];
    memset((uint8_t *)temp_3, ' ', 10);
    if (APP.APP_Oscillograph_Stop == 1)
        return;
    temp_ = (uint8_t)((1.0 * ADC_BUFFER_SIZE) / (lcddev.width - 12));
    switch (APP.Oscillograph_WorkType)
    {
    case Oscillograph:
        FFTTTask();
        sprintf((char *)temp_3, "%.2fKHz", APP.Wave_Detect_Frequency / 1000.0);
        Show_Str(8, 1, BLUE, BLACK, (uint8_t *)temp_3, 12, 0);
        memset((uint8_t *)temp_3, ' ', 10);
        sprintf((char *)temp_3, "%.3fV", APP.Wave_Detect_Max_Voltage);
        Show_Str(lcddev.width / 2 - 13, 1, BROWN, BLACK, (uint8_t *)temp_3, 12, 0);

        us_ = (uint32_t)(1000000.0 / APP.ADC_Frequency);
        memset(&us, ' ', 8);
        if (us_ >= 1000000)
        {
            sprintf((char *)us, " %.2fs ", us_ / 1000000.0);
        }
        else if (us_ >= 1000)
        {
            sprintf((char *)us, " %.2fms ", us_ / 1000.0);
        }
        else
        {
            sprintf((char *)us, " %dus ", us_);
        }
        Show_Str(lcddev.width / 2 - 20, lcddev.height - 13, BROWN, BLACK, (uint8_t *)us, 12, 0);
        Update_Force_Color(WHITE);

        switch (APP.APP_ADC_CHANNELS)
        {
        case CH0:
            Show_Str(5, lcddev.height - 13, BLACK, LIGHTBLUE, "   CH1   ", 12, 0);
            Show_Str(lcddev.width - 60, lcddev.height - 13, BROWN, BLACK, "   CH2   ", 12, 0);
            break;
        case CH1:
            Show_Str(5, lcddev.height - 13, BROWN, BLACK, "   CH1   ", 12, 0);
            Show_Str(lcddev.width - 60, lcddev.height - 13, BLACK, GREEN, "   CH2   ", 12, 0);
            break;
        case CH0_AND_CH1:
            Show_Str(5, lcddev.height - 13, BLACK, LIGHTBLUE, "   CH1   ", 12, 0);
            Show_Str(lcddev.width - 60, lcddev.height - 13, BLACK, GREEN, "   CH2   ", 12, 0);
            break;
        case CH0_FFT:
            Show_Str(5, lcddev.height - 13, BLACK, LIGHTBLUE, " CH1FFT ", 12, 0);
            Show_Str(lcddev.width - 60, lcddev.height - 13, BROWN, BLACK, "   CH2   ", 12, 0);
            break;
        case CH1_FFT:
            Show_Str(5, lcddev.height - 13, BROWN, BLACK, "   CH1   ", 12, 0);
            Show_Str(lcddev.width - 60, lcddev.height - 13, BLACK, GREEN, " CH2_FFT ", 12, 0);
            break;
        case CH0_AND_CH1_FFT:
            Show_Str(5, lcddev.height - 13, BLACK, LIGHTBLUE, " CH1_FFT ", 12, 0);
            Show_Str(lcddev.width - 60, lcddev.height - 13, BLACK, GREEN, " CH2_FFT ", 12, 0);
            break;
        default:
            break;
        }

        for (uint8_t j = 16; j < lcddev.height - 15; j += 15)
        {
            for (uint8_t i = 5; i < lcddev.width - 5; i += 4)
            {
                LCD_DrawPoint(i, j);
                LCD_DrawPoint(i + 1, j);
            }
        }
        for (uint8_t j = 5; j < lcddev.width - 5; j += 15)
        {
            for (uint8_t i = 16; i < lcddev.height - 15; i += 4)
            {
                LCD_DrawPoint(j, i);
                LCD_DrawPoint(j, i + 1);
            }
        }
        for (uint8_t i = 5; i < lcddev.width - 5; i += 3)
        {
            LCD_DrawLine(i, lcddev.height / 2 - 2, i, lcddev.height / 2 - 6);
        }
        for (uint8_t i = 16; i < lcddev.height - 15; i += 3)
        {
            LCD_DrawLine(lcddev.width / 2 - 2, i, lcddev.width / 2 + 2, i);
        }

        if (APP.ADC_Get_Ok_Flag == 1)
        {
            temp__ = 0;

            for (uint16_t i = 0; i < ADC_BUFFER_SIZE - 1; i += temp_)
            {
                if (i < 5 || i >= lcddev.width - 6)
                    continue;
                temp_1 = (uint8_t)(lcddev.height / 2 - 4 - APP.ADC_Ch_Buffer[temp__ * 2] / 4096.0 * 33);
                temp_2 = (uint8_t)(lcddev.height / 2 - 4 - APP.ADC_Ch_Buffer[temp__ * 2 + 1] / 4096.0 * 33);
                Update_Force_Color(BLACK);
                LCD_DrawPoint(6 + temp__, APP.ADC_Ch_Buffer_Temp[temp__ * 2]);
                LCD_DrawPoint(6 + temp__, APP.ADC_Ch_Buffer_Temp[temp__ * 2 + 1]);
                if (APP.APP_ADC_CHANNELS == CH0 || APP.APP_ADC_CHANNELS == CH0_AND_CH1 ||
                    APP.APP_ADC_CHANNELS == CH0_FFT || APP.APP_ADC_CHANNELS == CH0_AND_CH1_FFT)
                {
                    Update_Force_Color(LIGHTBLUE);
                    LCD_DrawPoint(6 + temp__, temp_1);
                }
                if (APP.APP_ADC_CHANNELS == CH1 || APP.APP_ADC_CHANNELS == CH0_AND_CH1 ||
                    APP.APP_ADC_CHANNELS == CH1_FFT || APP.APP_ADC_CHANNELS == CH0_AND_CH1_FFT)
                {
                    Update_Force_Color(GREEN);
                    LCD_DrawPoint(6 + temp__, temp_2);
                }
                APP.ADC_Ch_Buffer_Temp[temp__ * 2] = temp_1;
                APP.ADC_Ch_Buffer_Temp[temp__ * 2 + 1] = temp_2;
                temp__++;
            }
            APP_Start_ADC(APP.ADC_Frequency);
        }
        break;
    case Waveform_Generator:

        break;
    default:
        break;
    }
}
void APP_Init(void)
{
    DAC_Init();
    EX_TI_Init();
    APP.ADC_Handle_Clock = &htim3;
    APP.APP_Oscillograph_Stop = 0;
    APP.DAC_Handler = &DAC_Handler;
    APP.Wave_Type = Sin_Wave;
    APP.Sin_Wave_Frequency = 500;
    APP.APP_ADC_CHANNELS = CH0;
    APP.ADC_Frequency = 25000;
    APP.ADC_DMA_Handler = &hdma_adc1;
    APP.DAC_DMA_Handler = &hdma_dac_ch1;
    APP.ADC_DMA_Completed_CallBack = ADC_DMA_Completed_CallBack;
    APP.DeCoder_TIM_Callback = DeCoder_TIM_Callback;
    Decode_Init();
    Key_Init();
    APP.Decodee_handle = &Decodee_handle;
    APP.Is_FFT = 0;
    APP.Oscillograph_WorkType = Oscillograph;
    APP.APP_ADC_CHANNELS = CH0;
    Main_Windows_Init();
    APP.ADC_Get_Ok_Flag = 0;
    APP.Triangular_Wave_Duty_Cycle = 0.50;
    APP.Square_Wave_Duty_Cycle = 0.50;
    APP.Triangular_Wave_Frequency = 500;
    APP.Square_Wave_Frequency = 500;
}
void Reset_APP_Window_Resresh_Handler(void)
{
    APP.Window_Resresh_Handler = Window_Resresh_Handler;
}
// u16 65535   u32 4294967295
void APP_Start_ADC(uint16_t ADC_Frequency)
{
    uint32_t frequency = 72000000 / ADC_Frequency;
    uint16_t perid = 0;
    uint16_t perscaler = 0;
    if (frequency > 72 * 1000)
    {
        perscaler = 7200 - 1;
        perid = frequency / perscaler - 1;
    }
    else
    {
        perscaler = 72 - 1;
        perid = frequency / perscaler - 1;
    }
    HAL_ADC_Stop(&hadc1);
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_TIM_Base_Stop_IT(APP.ADC_Handle_Clock);
    __HAL_TIM_SET_PRESCALER(APP.ADC_Handle_Clock, perscaler);
    __HAL_TIM_SET_AUTORELOAD(APP.ADC_Handle_Clock, perid);
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_ADC_Start(&hadc1);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&(APP.ADC_Ch_Buffer), ADC_BUFFER_SIZE * 2);
    HAL_TIM_Base_Start_IT(APP.ADC_Handle_Clock);
    APP.ADC_Get_Ok_Flag = 0;
}
void APP_Stop_ADC(void)
{
    HAL_ADC_Stop(&hadc1);
    HAL_ADC_Stop_DMA(&hadc1);
    HAL_TIM_Base_Stop_IT(APP.ADC_Handle_Clock);
}
void ADC_DMA_Completed_CallBack(ADC_HandleTypeDef *hadc)
{
    APP.ADC_Get_Ok_Flag = 1;
    APP_Stop_ADC();
}
void DeCoder_TIM_Callback(TIM_HandleTypeDef *htim)
{
    printf("de\n");
}
void Main_Windows_Init(void)
{
    APP.Key_handler->BUtton_Cancel_Pressed_Handler = Select_Wave_Generate_Type_Windows;
    APP.Window_Resresh_Handler = Window_Resresh_Handler;
    APP.Key_handler->BUtton_OK_Pressed_Handler = Change_Channels;
    APP.Decodee_handle->DecodeC_Callback = Osci_Screenshot;
    LCD_Fill(0, 0, lcddev.width, lcddev.height, BLACK);
    LCD_SetWindows(5, 16, lcddev.width - 6, lcddev.height - 15);
    switch (APP.Oscillograph_WorkType)
    {
    case Waveform_Generator:

        break;
    case Oscillograph:
        Show_Str(lcddev.width - 40, 3, WHITE, BRED, " AUTO ", 12, 0);
        Update_Force_Color(WHITE);
        LCD_DrawRectangle(5, 16, lcddev.width - 5, lcddev.height - 15);
        Update_Force_Color(WHITE);
        LCD_DrawLine(5, lcddev.height / 2 - 4, lcddev.width - 5, lcddev.height / 2 - 4); // x
        LCD_DrawLine(lcddev.width / 2, 16, lcddev.width / 2, lcddev.height - 15);        // y
        APP.Decodee_handle->DecodeA_Callback = ADC_Frequency_Update;
        APP_Start_ADC(APP.ADC_Frequency);
        break;
    default:
        break;
    }
}
void ADC_Frequency_Update(void)
{
    if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_SET)
    {
        if (APP.ADC_Frequency <= 110 && APP.ADC_Frequency > 10)
        {
            APP.ADC_Frequency = APP.ADC_Frequency + 10;
        }
        else if (APP.ADC_Frequency <= 11)
        {
            APP.ADC_Frequency = APP.ADC_Frequency + 1;
        }
        else if (APP.ADC_Frequency < 5000)
            APP.ADC_Frequency = APP.ADC_Frequency + 100;
        else
            APP.ADC_Frequency = APP.ADC_Frequency < 65535 ? APP.ADC_Frequency + 1000 : APP.ADC_Frequency;
    }
    else if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_RESET)
    {
        if (APP.ADC_Frequency <= 110 && APP.ADC_Frequency > 10)
        {
            APP.ADC_Frequency = APP.ADC_Frequency > 11 ? APP.ADC_Frequency - 10 : APP.ADC_Frequency;
        }
        else if (APP.ADC_Frequency <= 11)
        {
            APP.ADC_Frequency = APP.ADC_Frequency > 2 ? APP.ADC_Frequency - 1 : APP.ADC_Frequency;
        }
        else if (APP.ADC_Frequency < 5000)
            APP.ADC_Frequency = APP.ADC_Frequency > 100 ? APP.ADC_Frequency - 100 : APP.ADC_Frequency;
        else if (APP.ADC_Frequency < 65535)
            APP.ADC_Frequency = APP.ADC_Frequency > 1000 ? APP.ADC_Frequency - 1000 : APP.ADC_Frequency;
    }
    printf("adc :%d \n", APP.ADC_Frequency);
    if (APP.ADC_Get_Ok_Flag == 0)
        APP_Start_ADC(APP.ADC_Frequency);
}
void DAC_Generate_Sin(uint16_t fre)
{
    HAL_TIM_Base_Stop_IT(APP.DAC_Handler->DAC_Handle_Clock);
    uint32_t frequency = 72000000 / (fre * MAX_DAC_BUFFER_SIZE);
    uint16_t perid = 0;
    uint16_t perscaler = 0;
    if (frequency > 72 * 1000)
    {
        perscaler = 7200;
        perid = frequency / perscaler - 1;
        perscaler -= 1;
    }
    else
    {
        perscaler = 72;
        perid = frequency / perscaler - 1;
        perscaler -= 1;
    }
    static uint32_t Idx;
    static float fr = 0.0f;
    fr = 2 * PI / MAX_DAC_BUFFER_SIZE;
    for (Idx = 0; Idx < MAX_DAC_BUFFER_SIZE; Idx++)
    {
        APP.DAC_Handler->Dac_S_T_Buffer[Idx] = (sin(Idx * fr) + 1) * 0.5 * ADC_Voltage;
    }
    __HAL_TIM_SET_PRESCALER(APP.DAC_Handler->DAC_Handle_Clock, perscaler);
    __HAL_TIM_SET_AUTORELOAD(APP.DAC_Handler->DAC_Handle_Clock, perid);
    APP.Sin_Wave_Frequency = fre;

    // HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&(APP.ADC_Ch_Buffer), ADC_BUFFER_SIZE * 2);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)APP.DAC_Handler->Dac_S_T_Buffer, MAX_DAC_BUFFER_SIZE, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start_IT(APP.DAC_Handler->DAC_Handle_Clock);
}
void DAC_Generate_Triangular(uint16_t fre, float duty)
{
    float frrx = (1.0 * ADC_Voltage) / (MAX_DAC_BUFFER_SIZE * duty);
    float frry = (1.0 * ADC_Voltage) / (MAX_DAC_BUFFER_SIZE * (1.0 - duty));
    uint16_t index = (uint16_t)(MAX_DAC_BUFFER_SIZE * duty);
    for (uint16_t i = 0; i < index; i++)
    {
        APP.DAC_Handler->Dac_S_T_Buffer[i] = (uint16_t)(i * frrx);
    }
    for (uint16_t i = index; i < MAX_DAC_BUFFER_SIZE; i++)
    {
        APP.DAC_Handler->Dac_S_T_Buffer[i] = 4095 - (i - index) * frry;
    }
    HAL_TIM_Base_Stop_IT(APP.DAC_Handler->DAC_Handle_Clock);
    uint32_t frequency = 72000000 / (fre * MAX_DAC_BUFFER_SIZE);
    uint16_t perid = 0;
    uint16_t perscaler = 0;
    if (frequency > 72 * 1000)
    {
        perscaler = 7200;
        perid = frequency / perscaler - 1;
        perscaler -= 1;
    }
    else
    {
        perscaler = 72;
        perid = frequency / perscaler - 1;
        perscaler -= 1;
    }

    __HAL_TIM_SET_PRESCALER(APP.DAC_Handler->DAC_Handle_Clock, perscaler);
    __HAL_TIM_SET_AUTORELOAD(APP.DAC_Handler->DAC_Handle_Clock, perid);
    APP.Triangular_Wave_Frequency = fre;
    APP.Triangular_Wave_Duty_Cycle = duty;
    // static uint32_t Idx;
    // for (Idx = 0; Idx < MAX_DAC_BUFFER_SIZE; Idx++)
    // {
    //     APP.DAC_Handler->Dac_S_T_Buffer[Idx] = (APP.DAC_Handler->Dac_S_T_Buffer[Idx] << 16) + (APP.DAC_Handler->Dac_S_T_Buffer[Idx]);
    // }
    // HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&(APP.ADC_Ch_Buffer), ADC_BUFFER_SIZE * 2);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)APP.DAC_Handler->Dac_S_T_Buffer, MAX_DAC_BUFFER_SIZE, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start_IT(APP.DAC_Handler->DAC_Handle_Clock);
}
void Show_Update_DAC_Window(void)
{
    // DrawUpdateDataWindows((uint32_t *)(&(APP.Sin_Wave_Frequency)), "update v(khz)", 14);
}
void DAC_Generate_Squre(uint16_t fre, float duty)
{
    uint16_t index = (uint16_t)(MAX_DAC_BUFFER_SIZE * duty);
    for (uint16_t i = 0; i < index; i++)
    {
        APP.DAC_Handler->Dac_S_T_Buffer[i] = ADC_Voltage - 1;
    }
    for (uint16_t i = index; i < MAX_DAC_BUFFER_SIZE; i++)
    {
        APP.DAC_Handler->Dac_S_T_Buffer[i] = 0;
    }
    HAL_TIM_Base_Stop_IT(APP.DAC_Handler->DAC_Handle_Clock);
    uint32_t frequency = 72000000 / (fre * MAX_DAC_BUFFER_SIZE);
    uint16_t perid = 0;
    uint16_t perscaler = 0;
    if (frequency > 72 * 1000)
    {
        perscaler = 7200;
        perid = frequency / perscaler - 1;
        perscaler = perscaler -1;
    }
    else
    {
        perscaler = 72;
        perid = frequency / perscaler - 1;
        perscaler = perscaler -1;
    }

    __HAL_TIM_SET_PRESCALER(APP.DAC_Handler->DAC_Handle_Clock, perscaler);
    __HAL_TIM_SET_AUTORELOAD(APP.DAC_Handler->DAC_Handle_Clock, perid);
    APP.Square_Wave_Frequency = fre;
    APP.Square_Wave_Duty_Cycle = duty;
    // static uint32_t Idx;
    // for (Idx = 0; Idx < MAX_DAC_BUFFER_SIZE; Idx++)
    // {
    //     APP.DAC_Handler->Dac_S_T_Buffer[Idx] = (APP.DAC_Handler->Dac_S_T_Buffer[Idx] << 16) + (APP.DAC_Handler->Dac_S_T_Buffer[Idx]);
    // }
    // HAL_ADC_Start_DMA(&hadc1, (uint32_t *)&(APP.ADC_Ch_Buffer), ADC_BUFFER_SIZE * 2);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)APP.DAC_Handler->Dac_S_T_Buffer, MAX_DAC_BUFFER_SIZE, DAC_ALIGN_12B_R);
    HAL_TIM_Base_Start_IT(APP.DAC_Handler->DAC_Handle_Clock);
}
void Change_Channels(void)
{
    switch (APP.APP_ADC_CHANNELS)
    {
    case CH0:
        APP.APP_ADC_CHANNELS = CH1;
        APP.Is_FFT = 0;
        break;
    case CH1:
        APP.APP_ADC_CHANNELS = CH0_AND_CH1;
        APP.Is_FFT = 0;
        break;
    case CH0_AND_CH1:
        APP.APP_ADC_CHANNELS = CH0_FFT;
        APP.Is_FFT = 0;
        break;
    case CH0_FFT:
        APP.APP_ADC_CHANNELS = CH1_FFT;
        APP.Is_FFT = 1;
        break;
    case CH1_FFT:
        APP.APP_ADC_CHANNELS = CH0_AND_CH1_FFT;
        APP.Is_FFT = 1;
        break;
    case CH0_AND_CH1_FFT:
        APP.APP_ADC_CHANNELS = CH0;
        APP.Is_FFT = 1;
        break;
    default:
        APP.APP_ADC_CHANNELS = CH0_AND_CH1;
        APP.Is_FFT = 0;
    }
}
void Osci_Screenshot(void)
{
    APP.APP_Oscillograph_Stop = APP.APP_Oscillograph_Stop == 1 ? 0 : 1;
}
void Select_Wave_Generate_Type_Windows(void)
{
    Key_Reset_Cancel_Handler();
    APP.Key_handler->BUtton_OK_Pressed_Handler = Close_Select_Wave_Generate_Type_Windows;
    APP.Window_Resresh_Handler = Refresh_Select_Wave_Generate_Type_Windows;
    APP.Decodee_handle->DecodeA_Callback = DeFault_Methode_Reset_CB;
    LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
    Update_Force_Color(GRAY);
    Show_Str(lcddev.width / 2 - 60, 5, BROWN, WHITE, "Wave Type Select", 16, 0);
    LCD_DrawRectangle(10, 30, lcddev.width - 10, lcddev.height - 20);

    LCD_DrawRectangle(15, 35, 15 + (lcddev.width - 20) / 3 - 4, 105);
    Update_Force_Color(BROWN);
    uint8_t len = (lcddev.width - 50) / 3;
    for (uint8_t i = 1; i < len; i++)
    {
        LCD_DrawPoint(19 + i, 55 + (int8_t)(18 * (sin(i * (2 * PI / len) + PI))));
    }
    Show_Str(22, 80, BLUE, WHITE, "SIN", 16, 0);

    // Update_Force_Color(GRAY);
    // LCD_DrawRectangle(15 + (lcddev.width - 20) / 3, 35, 15 + ((lcddev.width - 20) / 3 - 2) * 2 - 1, 105);
    // Update_Force_Color(BROWN);
    // LCD_DrawLine(22 + (lcddev.width - 20) / 3, 75, 22 + (lcddev.width - 20) / 3 + (lcddev.width - 20) / 9 - 10, 40);
    // LCD_DrawLine(22 + (lcddev.width - 20) / 3 + (lcddev.width - 20) / 9 - 10, 40, 22 + (lcddev.width - 20) / 3 + (lcddev.width - 20) / 9 * 2 - 10, 75);
    // LCD_DrawLine(22 + (lcddev.width - 20) / 3 + (lcddev.width - 20) / 9 * 2 - 10, 75, 22 + (lcddev.width - 20) / 3 + (lcddev.width - 20) / 9 * 2, 40);
    // Show_Str(22 + (lcddev.width - 20) / 3, 80, BLUE, WHITE, "TRI", 16, 0);

    // Update_Force_Color(GRAY);
    // LCD_DrawRectangle(15 + ((lcddev.width - 20) / 3 - 2) * 2 + 3, 35, 15 + ((lcddev.width - 20) / 3 - 2) * 3 - 1, 105);
    // Show_Str(22 + ((lcddev.width - 20) / 3 - 2) * 2 + 3, 80, BLUE, WHITE, "SQU", 16, 0);
    // Update_Force_Color(BROWN);
    // LCD_DrawLine(15 + ((lcddev.width - 20) / 3 - 2) * 2 + 3 + 2, 40, 15 + ((lcddev.width - 20) / 3 - 2) * 2 + 3 + 22, 40);
    // LCD_DrawLine(15 + ((lcddev.width - 20) / 3 - 2) * 2 + 3 + 22, 40, 15 + ((lcddev.width - 20) / 3 - 2) * 2 + 3 + 22, 75);
    // LCD_DrawLine(15 + ((lcddev.width - 20) / 3 - 2) * 2 + 3 + 22, 75, 15 + ((lcddev.width - 20) / 3 - 2) * 3 - 3, 75);

    Update_Force_Color(GRAY);
    LCD_DrawRectangle(15 + (lcddev.width - 20) / 3, 35, 15 + ((lcddev.width - 20) / 3 - 2) * 2 - 1, 105);
    Update_Force_Color(BROWN);
    LCD_DrawLine(65, 40, 80, 40);
    LCD_DrawLine(80, 40, 80, 75);
    LCD_DrawLine(80, 75, 95, 75);
    Show_Str(22 + (lcddev.width - 20) / 3, 80, BLUE, WHITE, "SQU", 16, 0);

    Update_Force_Color(GRAY);
    LCD_DrawRectangle(15 + ((lcddev.width - 20) / 3 - 2) * 2 + 3, 35, 15 + ((lcddev.width - 20) / 3 - 2) * 3 - 1, 105);
    Show_Str(22 + ((lcddev.width - 20) / 3 - 2) * 2 + 3, 80, BLUE, WHITE, "TRI", 16, 0);
    Update_Force_Color(BROWN);
    LCD_DrawLine(110, 40, 120, 75);
    LCD_DrawLine(120, 75, 130, 40);
    LCD_DrawLine(130, 40, 140, 75);

    APP.Decodee_handle->DecodeC_Callback = Change_Generate_Wave_Generate_Type;
}
void Refresh_Select_Wave_Generate_Type_Windows(void)
{

    if (APP.Wave_Type == Sin_Wave)
    {
        Update_Force_Color(BROWN);
        LCD_DrawRectangle(14, 34, 15 + (lcddev.width - 20) / 3 - 3, 106);
    }
    else
    {
        Update_Force_Color(WHITE);
        LCD_DrawRectangle(14, 34, 15 + (lcddev.width - 20) / 3 - 3, 106);
    }
    if (APP.Wave_Type == Square_Wave)
    {
        Update_Force_Color(BROWN);
        LCD_DrawRectangle(14 + (lcddev.width - 20) / 3, 34, 15 + ((lcddev.width - 20) / 3 - 2) * 2, 106);
    }
    else
    {
        Update_Force_Color(WHITE);
        LCD_DrawRectangle(14 + (lcddev.width - 20) / 3, 34, 15 + ((lcddev.width - 20) / 3 - 2) * 2, 106);
    }
    if (APP.Wave_Type == Triangular_Wave)
    {
        Update_Force_Color(BROWN);
        LCD_DrawRectangle(14 + ((lcddev.width - 20) / 3 - 2) * 2 + 3, 34, 15 + ((lcddev.width - 20) / 3 - 2) * 3, 106);
    }
    else
    {
        Update_Force_Color(WHITE);
        LCD_DrawRectangle(14 + ((lcddev.width - 20) / 3 - 2) * 2 + 3, 34, 15 + ((lcddev.width - 20) / 3 - 2) * 3, 106);
    }
}
void Close_Select_Wave_Generate_Type_Windows(void)
{
    switch (APP.Wave_Type)
    {
    case Sin_Wave:
        Config_Sin_Wave_Window_Init();
        break;
    case Triangular_Wave:
        Config_Triangular_Wave_Window_Init();
        break;
    case Square_Wave:
        Config_Squre_Wave_Window_Init();
        break;
    default:
        break;
    }

    // DrawUpdateDataWindows(&(APP.Sin_Wave_Frequency),"Update Fre(k)",14);
}
void Change_Generate_Wave_Generate_Type(void)
{
    switch (APP.Wave_Type)
    {
    case Sin_Wave:
        APP.Wave_Type = Square_Wave;
        break;
    case Square_Wave:
        APP.Wave_Type = Triangular_Wave;
        break;
    case Triangular_Wave:
        APP.Wave_Type = Sin_Wave;
        break;
    default:
        APP.Wave_Type = Sin_Wave;
        break;
    }
}
uint8_t Option_Select = 1;
void Change_Option(void)
{
    Option_Select++;
    Option_Select %= 3;
    Option_Select = Option_Select == 0 ? 1 : Option_Select;
}
void Config_Sin_Wave_Window_Init(void)
{
    APP.Window_Resresh_Handler = Refresh_Sin_Config_Windows;
    APP.Key_handler->BUtton_OK_Pressed_Handler = Set_Sin_Wave_Ok;
    APP.Key_handler->BUtton_Cancel_Pressed_Handler = Update_Sin_Wave_Config;
    LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
    Update_Force_Color(BLUE);
    LCD_DrawRectangle(5, 26, lcddev.width - 5, lcddev.height - 15);
    Show_Str(lcddev.width / 2 - 30, 2, BROWN, WHITE, "Sin Config", 16, 0);
    Show_Str(30, 36, BROWN, WHITE, "Freq: ", 16, 0);
    Show_Str(30, 56, BROWN, WHITE, "Ampl: ", 16, 0);
    Option_Select = 1;
    APP.Decodee_handle->DecodeC_Callback = Change_Option;
}
void Config_Triangular_Wave_Window_Init(void)
{
    APP.Window_Resresh_Handler = Refresh_Triangular_Config_Windows;
    APP.Key_handler->BUtton_OK_Pressed_Handler = Set_Triangular_Wave_Ok;
    APP.Key_handler->BUtton_Cancel_Pressed_Handler = Update_Triangular_Wave_Config;
    LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
    Update_Force_Color(BLUE);
    LCD_DrawRectangle(5, 26, lcddev.width - 5, lcddev.height - 15);
    Show_Str(lcddev.width / 2 - 65, 2, BROWN, WHITE, "Triangular Config", 16, 0);
    Show_Str(30, 36, BROWN, WHITE, "Freq: ", 16, 0);
    Show_Str(30, 56, BROWN, WHITE, "Duty:       %", 16, 0);
    Option_Select = 1;
    APP.Decodee_handle->DecodeC_Callback = Change_Option;
}
void Config_Squre_Wave_Window_Init(void)
{
    APP.Window_Resresh_Handler = Refresh_Squre_Config_Windows;
    APP.Key_handler->BUtton_OK_Pressed_Handler = Set_Squre_Wave_Ok;
    APP.Key_handler->BUtton_Cancel_Pressed_Handler = Update_Squre_Wave_Config;
    LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
    Update_Force_Color(BLUE);
    LCD_DrawRectangle(5, 26, lcddev.width - 5, lcddev.height - 15);
    Show_Str(lcddev.width / 2 - 50, 2, BROWN, WHITE, "Squre Config", 16, 0);
    Show_Str(30, 36, BROWN, WHITE, "Freq: ", 16, 0);
    Show_Str(30, 56, BROWN, WHITE, "Duty:       %", 16, 0);
    Option_Select = 1;
    APP.Decodee_handle->DecodeC_Callback = Change_Option;
}
void Refresh_Sin_Config_Windows(void)
{
    static uint8_t str[10] = {' '};
    memset((uint8_t *)str, ' ', 10);
    sprintf((char *)str, "%.3fKHz", APP.Sin_Wave_Frequency / 1000.0);
    if (Option_Select == 1)
    {
        Show_Str(10, 36, BLUE, WHITE, ">>", 16, 0);
        Show_Str(10, 56, WHITE, WHITE, ">>", 16, 0);
        Show_Str(80, 36, BROWN, WHITE, (uint8_t *)str, 16, 0);
        Show_Str(80, 56, BROWN, WHITE, "3.30V", 16, 0);
    }
    else if (Option_Select == 2)
    {
        Show_Str(10, 36, WHITE, WHITE, ">>", 16, 0);
        Show_Str(10, 56, BLUE, WHITE, ">>", 16, 0);
        Show_Str(80, 36, BROWN, WHITE, (uint8_t *)str, 16, 0);
        Show_Str(80, 56, BROWN, WHITE, "3.30V", 16, 0);
    }
}
void Update_Triangle_Duty(void)
{
    if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_SET)
    {
        APP.Triangular_Wave_Duty_Cycle += 0.01;
        APP.Triangular_Wave_Duty_Cycle = APP.Triangular_Wave_Duty_Cycle >= 1 ? 0.99 : APP.Triangular_Wave_Duty_Cycle;
    }
    else if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_RESET)
    {
        APP.Triangular_Wave_Duty_Cycle -= 0.01;
        APP.Triangular_Wave_Duty_Cycle = APP.Triangular_Wave_Duty_Cycle <= 0 ? 0.01 : APP.Triangular_Wave_Duty_Cycle;
    }
}
void Update_Squre_Duty(void)
{
    if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_SET)
    {
        APP.Square_Wave_Duty_Cycle += 0.01;
        APP.Square_Wave_Duty_Cycle = APP.Square_Wave_Duty_Cycle >= 1 ? 0.99 : APP.Square_Wave_Duty_Cycle;
    }
    else if (HAL_GPIO_ReadPin(DecodeB_GPIO_Port, DecodeB_Pin) == GPIO_PIN_RESET)
    {
        APP.Square_Wave_Duty_Cycle -= 0.01;
        APP.Square_Wave_Duty_Cycle = APP.Square_Wave_Duty_Cycle <= 0 ? 0.01 : APP.Square_Wave_Duty_Cycle;
    }
}
void Refresh_Triangular_Config_Windows(void)
{
    static uint8_t str[10] = {' '};
    memset((uint8_t *)str, ' ', 10);
    sprintf((char *)str, "%.3fKHz", APP.Triangular_Wave_Frequency / 1000.0);
    if (Option_Select == 1)
    {
        Show_Str(10, 36, BLUE, WHITE, ">>", 16, 0);
        Show_Str(10, 56, WHITE, WHITE, ">>", 16, 0);
        Show_Str(80, 36, BROWN, WHITE, (uint8_t *)str, 16, 0);
        memset((uint8_t *)str, ' ', 10);
        sprintf((char *)str, "%.2f", APP.Triangular_Wave_Duty_Cycle * 100);
        Show_Str(80, 56, BROWN, WHITE, (uint8_t *)str, 16, 0);
        APP.Decodee_handle->DecodeA_Callback = DeFault_Methode_Reset_CB;
    }
    else if (Option_Select == 2)
    {
        Show_Str(10, 36, WHITE, WHITE, ">>", 16, 0);
        Show_Str(10, 56, BLUE, WHITE, ">>", 16, 0);
        Show_Str(80, 36, BROWN, WHITE, (uint8_t *)str, 16, 0);
        memset((uint8_t *)str, ' ', 10);
        sprintf((char *)str, "%.2f", APP.Triangular_Wave_Duty_Cycle * 100);
        Show_Str(80, 56, BROWN, WHITE, (uint8_t *)str, 16, 0);
        APP.Decodee_handle->DecodeA_Callback = Update_Triangle_Duty;
    }
}
void Refresh_Squre_Config_Windows(void)
{
    static uint8_t str[10] = {' '};
    memset((uint8_t *)str, ' ', 10);
    sprintf((char *)str, "%.3fKHz", APP.Square_Wave_Frequency / 1000.0);
    if (Option_Select == 1)
    {
        Show_Str(10, 36, BLUE, WHITE, ">>", 16, 0);
        Show_Str(10, 56, WHITE, WHITE, ">>", 16, 0);
        Show_Str(80, 36, BROWN, WHITE, (uint8_t *)str, 16, 0);
        memset((uint8_t *)str, ' ', 10);
        sprintf((char *)str, "%.2f", APP.Square_Wave_Duty_Cycle * 100);
        Show_Str(80, 56, BROWN, WHITE, (uint8_t *)str, 16, 0);
        APP.Decodee_handle->DecodeA_Callback = DeFault_Methode_Reset_CB;
    }
    else if (Option_Select == 2)
    {
        Show_Str(10, 36, WHITE, WHITE, ">>", 16, 0);
        Show_Str(10, 56, BLUE, WHITE, ">>", 16, 0);
        Show_Str(80, 36, BROWN, WHITE, (uint8_t *)str, 16, 0);
        memset((uint8_t *)str, ' ', 10);
        sprintf((char *)str, "%.2f", APP.Square_Wave_Duty_Cycle * 100);
        Show_Str(80, 56, BROWN, WHITE, (uint8_t *)str, 16, 0);
        APP.Decodee_handle->DecodeA_Callback = Update_Squre_Duty;
    }
}
void Set_Sin_Wave_Ok(void)
{
    Main_Windows_Init();
    DAC_Generate_Sin(APP.Sin_Wave_Frequency);
}
void Set_Triangular_Wave_Ok(void)
{
    Main_Windows_Init();
    DAC_Generate_Triangular(APP.Triangular_Wave_Frequency, APP.Triangular_Wave_Duty_Cycle);
}
void Set_Squre_Wave_Ok(void)
{
    Main_Windows_Init();
    DAC_Generate_Squre(APP.Square_Wave_Frequency, APP.Square_Wave_Duty_Cycle);
}
void Close_Update_Sin_Wave_Config(void)
{
    Config_Sin_Wave_Window_Init();
}
void Close_Update_Triangular_Wave_Config(void)
{
    Config_Triangular_Wave_Window_Init();
}
void Close_Update_Squre_Wave_Config(void)
{
    Config_Squre_Wave_Window_Init();
}
void Update_Sin_Wave_Config(void)
{
    if (Option_Select == 1)
    {
        DrawUpdateDataWindows(&(APP.Sin_Wave_Frequency), "Frequency", 10, Close_Update_Sin_Wave_Config);
    }
    else if (Option_Select == 2)
    {
    }
}
void Update_Triangular_Wave_Config(void)
{
    if (Option_Select == 1)
    {
        DrawUpdateDataWindows(&(APP.Triangular_Wave_Frequency), "Frequency", 10, Close_Update_Triangular_Wave_Config);
    }
    else if (Option_Select == 2)
    {
        // DrawUpdateDataWindows(&(APP.Triangular_Wave_Duty_Cycle), "Duty", 10, Close_Update_Triangular_Wave_Config);
    }
}
void Update_Squre_Wave_Config(void)
{
    if (Option_Select == 1)
    {
        DrawUpdateDataWindows(&(APP.Square_Wave_Frequency), "Frequency", 10, Close_Update_Squre_Wave_Config);
    }
    else if (Option_Select == 2)
    {
        // DrawUpdateDataWindows(&(APP.Triangular_Wave_Duty_Cycle), "Duty", 10, Close_Update_Triangular_Wave_Config);
    }
}
void FFTTTask(void)
{
    static uint16_t j;
    switch (APP.APP_ADC_CHANNELS)
    {
    case CH0_FFT:
        for (j = 0; j < ADC_BUFFER_SIZE; j++)
        {
            APP.ADC_Ch0_Collection[j].real = APP.ADC_Ch_Buffer[j * 2];
            APP.ADC_Ch0_Collection[j].imag = 0;
        }
        FFT(APP.ADC_Ch0_Collection);
        for (j = 0; j < ADC_BUFFER_SIZE; j++)
        {
            APP.ADC_Ch_Buffer[j * 2] = (uint16_t)sqrt(APP.ADC_Ch0_Collection[j].real * APP.ADC_Ch0_Collection[j].real + APP.ADC_Ch0_Collection[j].imag + APP.ADC_Ch0_Collection[j].imag);
        }
        break;
    case CH1_FFT:
        for (j = 0; j < ADC_BUFFER_SIZE; j++)
        {
            APP.ADC_Ch1_Collection[j].real = APP.ADC_Ch_Buffer[j * 2 + 1];
            APP.ADC_Ch1_Collection[j].imag = 0;
        }
        FFT(APP.ADC_Ch1_Collection);
        for (j = 0; j < ADC_BUFFER_SIZE; j++)
        {
            APP.ADC_Ch_Buffer[j * 2 + 1] = (uint16_t) sqrt(APP.ADC_Ch1_Collection[j].real * APP.ADC_Ch1_Collection[j].real + APP.ADC_Ch1_Collection[j].imag + APP.ADC_Ch1_Collection[j].imag);
        }
        break;
    case CH0_AND_CH1_FFT:
        for (j = 0; j < ADC_BUFFER_SIZE; j++)
        {
            APP.ADC_Ch1_Collection[j].real = APP.ADC_Ch_Buffer[j * 2 + 1];
            APP.ADC_Ch1_Collection[j].imag = 0;
            APP.ADC_Ch0_Collection[j].real = APP.ADC_Ch_Buffer[j * 2];
            APP.ADC_Ch0_Collection[j].imag = 0;
        }
        FFT(APP.ADC_Ch1_Collection);
        FFT(APP.ADC_Ch0_Collection);
        for (j = 0; j < ADC_BUFFER_SIZE; j++)
        {
            APP.ADC_Ch_Buffer[j * 2 + 1] =
                 (uint16_t)sqrt(APP.ADC_Ch1_Collection[j].real * APP.ADC_Ch1_Collection[j].real + APP.ADC_Ch1_Collection[j].imag + APP.ADC_Ch1_Collection[j].imag);
            APP.ADC_Ch_Buffer[j * 2] =
                 (uint16_t)sqrt(APP.ADC_Ch0_Collection[j].real * APP.ADC_Ch0_Collection[j].real + APP.ADC_Ch0_Collection[j].imag + APP.ADC_Ch0_Collection[j].imag);
        }
        break;
    default:
        break;
    }
}
