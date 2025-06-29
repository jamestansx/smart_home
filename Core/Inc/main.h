/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct {
	float temp;
	float humid;
	float pressure;
} SensorData_t;
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
#define WIFI_RESET_Pin GPIO_PIN_8
#define WIFI_RESET_GPIO_Port GPIOE
#define WIFI_NSS_Pin GPIO_PIN_0
#define WIFI_NSS_GPIO_Port GPIOE
#define WIFI_CMD_DATA_READY_Pin GPIO_PIN_1
#define WIFI_CMD_DATA_READY_GPIO_Port GPIOE
#define WIFI_CMD_DATA_READY_EXTI_IRQn EXTI1_IRQn
/* USER CODE BEGIN Private defines */
#define SENSORDATA_OBJS 12
#define SSID "DT105_2.4GHz@unifi"
#define PASSPHRASE "112233DT"

#define MQTT_ADDRESS "192.168.0.35"
#define PUBLISH_TOPIC "testing"
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
