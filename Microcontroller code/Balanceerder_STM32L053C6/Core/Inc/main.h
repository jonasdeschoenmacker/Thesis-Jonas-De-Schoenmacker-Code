/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l0xx_hal.h"

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
#define Get_data_Pin GPIO_PIN_1
#define Get_data_GPIO_Port GPIOA
#define EN_flyback_Pin GPIO_PIN_2
#define EN_flyback_GPIO_Port GPIOA
#define CLK4_Pin GPIO_PIN_6
#define CLK4_GPIO_Port GPIOA
#define CLK3_Pin GPIO_PIN_7
#define CLK3_GPIO_Port GPIOA
#define CLK2_Pin GPIO_PIN_0
#define CLK2_GPIO_Port GPIOB
#define CLK1_Pin GPIO_PIN_1
#define CLK1_GPIO_Port GPIOB
#define Blue_LED_Pin GPIO_PIN_12
#define Blue_LED_GPIO_Port GPIOB
#define Green_LED_Pin GPIO_PIN_13
#define Green_LED_GPIO_Port GPIOB
#define Red_LED_Pin GPIO_PIN_14
#define Red_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
