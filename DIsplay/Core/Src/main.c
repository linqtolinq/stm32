/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "./lcd/bsp_ili9341_lcd.h"
#include "stdio.h"
#include "task.h"
#include "LedTask.h"
#include "key.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void LCD_Show(void);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint16_t lcdid;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	ILI9341_Init (); 
  key_gpio_nint();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	 if(lcdid == LCDID_ILI9341)
  {
    ILI9341_GramScan ( 5 );
  }
  else if(lcdid == LCDID_ST7789V)
  {
    ILI9341_GramScan ( 0 );
  }
	LCD_Show();
	printf("init ok\r\n");

   
//  HAL_ADC_Start_IT(&hadc1);
//	ADC_Start();

  update_adc_sampling_frequency(1000);
  while (1)
  {

    if(systick_falg == 1)
    {
      systick_falg = 0;
      Task_Tick();
    }
    if(adc_completed_Flag == 1)
    {
      adc_completed_Flag = 0;
      Data_Progress();
      Drew_sin_wave_Tran();
      data_show();
      ADC_Start();
    }
    // if(adc_Flag == 1 && adc_completed_Flag!= 1)
    // {
    //   adc_Flag = 0;
    //   // ADC_Start();
    // }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void LCD_Show(void)
{
	ILI9341_BackLed_Control(ENABLE);
	

	LCD_SetTextColor(WHITE);
	ILI9341_DrawRectangle(0,0,320,240,1);
	
	
	LCD_SetColors(BLUE,WHITE);//??
	LCD_SetFont(&Font8x16);
  ILI9341_DrawRectangle(5,5,310,230,0);
	ILI9341_DrawRectangle(6,6,308,228,0);
	ILI9341_DrawLine(6,66,314,66);
	ILI9341_DrawLine(6,36,314,36);
	ILI9341_DrawLine(6,36,314,36);
	ILI9341_DrawLine(108,6,108,66);
	ILI9341_DrawLine(211,6,211,66);
	LCD_SetColors(WHITE,BLUE);

	
	
	LCD_SetColors(BLUE,WHITE);//??
	ILI9341_DispString_EN(11,11,"U:");
	ILI9341_DispString_EN(95,11,"V");
	ILI9341_DispString_EN(113,11,"CH1:");
	ILI9341_DrawLine(150,20,200,20);
	LCD_SetColors(RED,WHITE);
	ILI9341_DispString_EN(216,11,"CH2:");
	ILI9341_DrawLine(253,20,303,20);
	LCD_SetColors(BLUE,WHITE);
	ILI9341_DispString_EN(11,41,"Um:");
	ILI9341_DispString_EN(95,41,"V");
	ILI9341_DispString_EN(113,41,"T:");
	ILI9341_DispString_EN(190,41,"ms");
	ILI9341_DispString_EN(216,41,"f:");
	ILI9341_DispString_EN(293,41,"Hz");
	
	LCD_SetColors(BLUE,WHITE);
  



	
	
	// LCD_SetColors(BLUE,WHITE);//??? ???(16,149)
	// ILI9341_DrawLine(16,71,16,224);//Y?
	// ILI9341_DrawLine(16,224-75,309,224-75);//X?
	// ILI9341_DrawLine(16,71,11,81);//Y???
	// ILI9341_DrawLine(16,71,21,81);
	// ILI9341_DispString_EN(25,71,"Y");
	// ILI9341_DrawLine(309,224-75,299,219-75);//X???
	// ILI9341_DrawLine(309,224-75,299,229-75);
	// ILI9341_DispString_EN(301,203-75,"X");
	// ILI9341_DrawRectangle(24,84,270,140,1);
	
	LCD_SetColors(BLUE,WHITE);
	LCD_SetFont(&Font16x24);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
