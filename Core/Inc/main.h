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
#include "stm32f4xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define cyl_dir1_Pin GPIO_PIN_0
#define cyl_dir1_GPIO_Port GPIOC
#define cyl_dir2_Pin GPIO_PIN_1
#define cyl_dir2_GPIO_Port GPIOC
#define cyl_dir3_Pin GPIO_PIN_2
#define cyl_dir3_GPIO_Port GPIOC
#define cyl_dir4_Pin GPIO_PIN_3
#define cyl_dir4_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define cyl_dir5_Pin GPIO_PIN_4
#define cyl_dir5_GPIO_Port GPIOC
#define cyl_dir6_Pin GPIO_PIN_5
#define cyl_dir6_GPIO_Port GPIOC
#define led_ride_stop_Pin GPIO_PIN_12
#define led_ride_stop_GPIO_Port GPIOB
#define led_ride_start_Pin GPIO_PIN_13
#define led_ride_start_GPIO_Port GPIOB
#define button_ride_stop_Pin GPIO_PIN_14
#define button_ride_stop_GPIO_Port GPIOB
#define button_ride_start_Pin GPIO_PIN_15
#define button_ride_start_GPIO_Port GPIOB
#define cyl_pwm5_Pin GPIO_PIN_6
#define cyl_pwm5_GPIO_Port GPIOC
#define cyl_pwm6_Pin GPIO_PIN_7
#define cyl_pwm6_GPIO_Port GPIOC
#define unused_pwm1_Pin GPIO_PIN_8
#define unused_pwm1_GPIO_Port GPIOC
#define unused_pwm2_Pin GPIO_PIN_9
#define unused_pwm2_GPIO_Port GPIOC
#define cyl_pwm1_Pin GPIO_PIN_8
#define cyl_pwm1_GPIO_Port GPIOA
#define cyl_pwm2_Pin GPIO_PIN_9
#define cyl_pwm2_GPIO_Port GPIOA
#define cyl_pwm3_Pin GPIO_PIN_10
#define cyl_pwm3_GPIO_Port GPIOA
#define cyl_pwm4_Pin GPIO_PIN_11
#define cyl_pwm4_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
