/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#define TX2_Pin GPIO_PIN_2
#define TX2_GPIO_Port GPIOA
#define WBRESET_Pin GPIO_PIN_3
#define WBRESET_GPIO_Port GPIOA
#define MIDI_WB_CON_Pin GPIO_PIN_4
#define MIDI_WB_CON_GPIO_Port GPIOA
#define MIDI_OUT_CON_Pin GPIO_PIN_5
#define MIDI_OUT_CON_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_0
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_1
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_2
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_10
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_11
#define LED5_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_12
#define LED6_GPIO_Port GPIOB
#define LED7_Pin GPIO_PIN_13
#define LED7_GPIO_Port GPIOB
#define LED8_Pin GPIO_PIN_14
#define LED8_GPIO_Port GPIOB
#define TX1_Pin GPIO_PIN_9
#define TX1_GPIO_Port GPIOA
#define RX1_Pin GPIO_PIN_10
#define RX1_GPIO_Port GPIOA
#define ENC_A_Pin GPIO_PIN_5
#define ENC_A_GPIO_Port GPIOB
#define ENC_B_Pin GPIO_PIN_6
#define ENC_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
