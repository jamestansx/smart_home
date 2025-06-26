/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os2.h"
#include "i2c.h"
#include "quadspi.h"
#include "spi.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "stm32l4xx.h"
#include "stm32l475e_iot01_tsensor.h"
#include "stm32l475e_iot01_hsensor.h"
#include "stm32l475e_iot01_psensor.h"
#include "stm32l475e_iot01.h"
#include "wifi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define TX_BUFFER_SIZE 1024
#define RX_BUFFER_SIZE 1024
#define TX_TIMEOUT 2000
#define RX_TIMEOUT 2000



#define JSON_DATA_TEMPL "{\"temperature\": %s, \"humidity\": %s, \"pressure\": %s}"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
WIFI_HandleTypeDef hwifi;
__IO FlagStatus cmdDataReady = 0;
volatile bool wifi_initialized = false;

extern osMutexId_t spi_mutex_id;
extern osMessageQueueId_t msgQueue_id;
extern osSemaphoreId_t wifi_init_done_sem;
extern osSemaphoreId_t wifi_data_ready_sem;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
	MX_SPI3_Init();
	MX_USART1_UART_Init();
	MX_QUADSPI_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */
	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();

	MX_FREERTOS_Init();

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 40;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1
			| RCC_PERIPHCLK_I2C2;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
	PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */
static void WIFI_Init_main() {
	hwifi.handle = &hspi3;
	hwifi.ssid = SSID;
	hwifi.passphrase = PASSPHRASE;
	hwifi.securityType = WPA_MIXED + 1;
	hwifi.DHCP = SET;
	WIFI_Init(&hwifi);
	WIFI_JoinNetwork(&hwifi);

	printf("IP ADDRESS: %s\n", hwifi.ipAddress);

	sprintf(hwifi.mqtt.publishTopic, PUBLISH_TOPIC);
	hwifi.mqtt.securityMode = WIFI_MQTT_SECURITY_NONE;
	hwifi.mqtt.keepAlive = 3000;
	sprintf(hwifi.remoteIpAddress, MQTT_ADDRESS);
	hwifi.port = 1883;
	WIFI_StatusTypeDef status = WIFI_MQTTClientInit(&hwifi);
	printf("WIFI INIT DONE: %d\n", status);

	char buf[] = "WTF";
	WIFI_MQTTPublish(&hwifi, buf, sizeof(buf));
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
        if (GPIO_Pin == WIFI_CMD_DATA_READY_Pin) {
                cmdDataReady = HAL_GPIO_ReadPin(WIFI_CMD_DATA_READY_GPIO_Port, WIFI_CMD_DATA_READY_Pin);
        }
}

void ReadTemperature(void *argument) {
	/* USER CODE BEGIN ReadTemperature */
	SensorData_t data;
	char temp_str[10];
	char humid_str[10];
	char pressure_str[10];
	char buf[256];

	for (;;) {
		float temp = BSP_TSENSOR_ReadTemp();
		float humid = BSP_HSENSOR_ReadHumidity();
		float pressure = BSP_PSENSOR_ReadPressure();
		data.temp = temp;
		data.humid = humid;
		data.pressure = pressure;

		int temp1 = data.temp;
		float tempFrac = data.temp - temp1;
		int temp2 = trunc(tempFrac * 100);
		snprintf(temp_str, sizeof(temp_str), "%d.%02d", temp1, temp2);
		int humid1 = data.humid;
		float humidFrac = data.humid - humid1;
		int humid2 = trunc(humidFrac * 100);
		snprintf(humid_str, sizeof(humid_str), "%d.%02d", humid1, humid2);
		int pressure1 = data.pressure;
		float pressureFrac = data.pressure - pressure1;
		int pressure2 = trunc(pressureFrac * 100);
		snprintf(pressure_str, sizeof(pressure_str), "%d.%02d", pressure1,
				pressure2);

		snprintf(buf, sizeof(buf), JSON_DATA_TEMPL, temp_str, humid_str, pressure_str);
		printf("Output: %s\n", buf);

		if (osMutexAcquire(spi_mutex_id, 1000) == osOK) {
			WIFI_MQTTPublish(&hwifi, buf, sizeof(buf));
			osMutexRelease(spi_mutex_id);
		}
		osDelay(10);
	}
	/* USER CODE END ReadTemperature */
}

void WifiInit(void *argument) {
        /* USER CODE BEGIN WifiInit */
	WIFI_Init_main();
	wifi_initialized = true;
	osSemaphoreRelease(wifi_init_done_sem);
	osThreadSuspend(osThreadGetId());

	while (1) {
		osDelay(osWaitForever);
	}
	/* USER CODE END WifiInit */
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
