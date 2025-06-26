/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId_t WifiHandle;
const osThreadAttr_t Wifi_attrs = { .name = "Wifi", .priority =
		(osPriority_t) osPriorityHigh, .stack_size = 4048, };

osThreadId_t TemperatureHandle;
const osThreadAttr_t Temperature_attributes =
		{ .name = "Temperature", .priority =
				(osPriority_t) osPriorityNormal, .stack_size = 128 * 10 };

osMutexId_t spi_mutex_id;
osMessageQueueId_t msgQueue_id;
osSemaphoreId_t wifi_init_done_sem;
osSemaphoreId_t wifi_data_ready_sem;
/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void ReadTemperature(void *argument);
void WifiInit(void *argument);
/* USER CODE END FunctionPrototypes */

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
	/* USER CODE BEGIN Init */
	BSP_TSENSOR_Init();
	BSP_HSENSOR_Init();
	BSP_PSENSOR_Init();
	/* USER CODE END Init */

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	TemperatureHandle = osThreadNew(ReadTemperature, NULL, &Temperature_attributes);
	WifiHandle = osThreadNew(WifiInit, NULL, &Wifi_attrs);

	msgQueue_id = osMessageQueueNew(SENSORDATA_OBJS, sizeof(SensorData_t), NULL);
	if (msgQueue_id == NULL) printf("FUCKUP\n");

	spi_mutex_id = osMutexNew(NULL);
	if (spi_mutex_id == NULL) printf("SPI mutex failed to create!!!\n");

	wifi_init_done_sem = osSemaphoreNew(1, 0, NULL);
	if (wifi_init_done_sem == NULL) printf("semaphore failed to create!!!\n");

	wifi_data_ready_sem = osSemaphoreNew(1, 0, NULL);
	if (wifi_data_ready_sem == NULL) printf("semaphore failed to create!!!\n");

	/* USER CODE END RTOS_THREADS */

}
/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
