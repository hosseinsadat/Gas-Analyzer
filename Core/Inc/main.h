/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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
#include "xpt2046.h"

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define T_CS_Pin GPIO_PIN_8
#define T_CS_GPIO_Port GPIOI
#define LCD_PWR_Pin GPIO_PIN_2
#define LCD_PWR_GPIO_Port GPIOH
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define T_SCK_Pin GPIO_PIN_6
#define T_SCK_GPIO_Port GPIOH
#define T_IRQ_Pin GPIO_PIN_7
#define T_IRQ_GPIO_Port GPIOH
#define CO_MEB_Pin GPIO_PIN_13
#define CO_MEB_GPIO_Port GPIOB
#define O2_MEB_Pin GPIO_PIN_14
#define O2_MEB_GPIO_Port GPIOB
#define T_MISO_Pin GPIO_PIN_3
#define T_MISO_GPIO_Port GPIOG
#define CO2_IO_Pin GPIO_PIN_10
#define CO2_IO_GPIO_Port GPIOA
#define T_MOSI_Pin GPIO_PIN_3
#define T_MOSI_GPIO_Port GPIOI
#define NRF_IRQ_Pin GPIO_PIN_2
#define NRF_IRQ_GPIO_Port GPIOD
#define NRF_CSN_Pin GPIO_PIN_3
#define NRF_CSN_GPIO_Port GPIOD
#define MAX_CS_Pin GPIO_PIN_12
#define MAX_CS_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
