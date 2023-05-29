/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DecodeA_Pin GPIO_PIN_0
#define DecodeA_GPIO_Port GPIOC
#define DecodeA_EXTI_IRQn EXTI0_IRQn
#define DecodeB_Pin GPIO_PIN_1
#define DecodeB_GPIO_Port GPIOC
#define DecodeC_Pin GPIO_PIN_2
#define DecodeC_GPIO_Port GPIOC
#define DecodeC_EXTI_IRQn EXTI2_IRQn
#define Button_OK_Pin GPIO_PIN_3
#define Button_OK_GPIO_Port GPIOC
#define Button_OK_EXTI_IRQn EXTI3_IRQn
#define Button_Cancel_Pin GPIO_PIN_4
#define Button_Cancel_GPIO_Port GPIOC
#define Button_Cancel_EXTI_IRQn EXTI4_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
