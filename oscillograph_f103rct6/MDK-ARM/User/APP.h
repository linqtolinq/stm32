#ifndef __APP_H
#define __APP_H

#include "main.h"
#include "Decode.h"
#include "key.h"
#include "dac_task.h"
#include "stm32f1xx_it.h"
#include "FFT.h"
#define ADC_BUFFER_SIZE 256
#define ADC_Voltage 4096

typedef enum
{
    Sin_Wave = 0,
    Square_Wave = 1,
    Triangular_Wave = 2
} Generic_Wave_Type;

typedef enum
{
    Waveform_Generator = 0,
    Oscillograph = 1,
} WorkType;

typedef enum
{
    CH0 = 0,
    CH1 = 1,
    CH0_AND_CH1 = 2,
    CH0_FFT = 3,
    CH1_FFT = 4,
    CH0_AND_CH1_FFT = 5,
} ADC_CHANNELS;

typedef struct
{

    TIM_HandleTypeDef *ADC_Handle_Clock;
    DMA_HandleTypeDef *ADC_DMA_Handler;
    DAC_Handle *DAC_Handler;
    DMA_HandleTypeDef *DAC_DMA_Handler;

    uint16_t ADC_Ch_Buffer[ADC_BUFFER_SIZE * 2];
    uint8_t ADC_Ch_Buffer_Temp[ADC_BUFFER_SIZE * 2];
    compx ADC_Ch0_Collection[ADC_BUFFER_SIZE];
    compx ADC_Ch1_Collection[ADC_BUFFER_SIZE];
    // uint16_t ADC_Ch0_Collection_Temp[ADC_BUFFER_SIZE];
    // uint16_t ADC_Ch1_Collection_Temp[ADC_BUFFER_SIZE];
    uint32_t ADC_Frequency;
    uint32_t Wave_Detect_Frequency;
    float Wave_Detect_Max_Voltage;
    uint8_t ADC_Get_Ok_Flag;

    /// @brief the wave type for oscillograph to generate
    Generic_Wave_Type Wave_Type;

    /// @brief square wave frequency for oscillograph to generate
    uint32_t Square_Wave_Frequency;
    /// @brief square wave duty cycle for oscillograph to generate
    float Square_Wave_Duty_Cycle;

    /// @brief sin wave frequency for oscillograph to generate
    uint32_t Sin_Wave_Frequency;

    /// @brief Triangular wave frequency for oscillograph to generate
    uint32_t Triangular_Wave_Frequency;
    /// @brief Triangular wave duty cycle for oscillograph to generate
    float Triangular_Wave_Duty_Cycle;

    uint8_t APP_Oscillograph_Stop;

    WorkType Oscillograph_WorkType;

    ADC_CHANNELS APP_ADC_CHANNELS;
    void (*ADC_DMA_Completed_CallBack)(ADC_HandleTypeDef *hadc);
    void (*DeCoder_TIM_Callback)(TIM_HandleTypeDef *htim);

    Decoder_Handle *Decodee_handle;
    Key_Handler *Key_handler;

    EX_TI_Flag *EX_TI_Flags;

    void (*Window_Resresh_Handler)(void);
    void (*Window_Init_Handler)(void);
    uint8_t Is_FFT;
} APP_Demo;

extern APP_Demo APP;

void ADC_DMA_Completed_CallBack(ADC_HandleTypeDef *hdma);
void Reset_APP_Window_Resresh_Handler(void);
void DeCoder_TIM_Callback(TIM_HandleTypeDef *htim);
void APP_Start_ADC(uint16_t ADC_Frequency);
void APP_Init(void);
void APP_Stop_ADC(void);
void Main_Windows_Init(void);
void ADC_Frequency_Update(void);
void DAC_Generate_Sin(uint16_t fre);
void Show_Update_DAC_Window(void);
void Change_Channels(void);
void Osci_Screenshot(void);
void Select_Wave_Generate_Type_Windows(void);
void Refresh_Select_Wave_Generate_Type_Windows(void);
void Close_Select_Wave_Generate_Type_Windows(void);
void Change_Generate_Wave_Generate_Type(void);

void Config_Sin_Wave_Window_Init(void);
void Refresh_Sin_Config_Windows(void);
void Set_Sin_Wave_Ok(void);
void Update_Sin_Wave_Config(void);

void Config_Triangular_Wave_Window_Init(void);
void Refresh_Triangular_Config_Windows(void);
void Set_Triangular_Wave_Ok(void);
void Update_Triangular_Wave_Config(void);

void Config_Squre_Wave_Window_Init(void);
void Refresh_Squre_Config_Windows(void);
void Set_Squre_Wave_Ok(void);
void Update_Squre_Wave_Config(void);
void FFTTTask(void);
#endif
