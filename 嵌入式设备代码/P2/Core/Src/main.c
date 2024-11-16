/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include <string.h>
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

/* USER CODE BEGIN PV */
uint8_t lcdBuffer[21];
uint8_t lcdMs;
uint8_t button = 0;
uint8_t romWriteBuffer;
uint8_t romReadBuffer;
uint8_t ucCNT = 5;
uint8_t test;
uint8_t usartBuffer;
uint8_t usartData[20];
uint8_t usartDataNum;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t keyRead(void);
void keyFunc(void);
void lcdFunc(void);
void usartFunc(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t keyRead(void)
{
  uint8_t value = 0;
  if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == 0)
  {
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0) == 0)
    {
      value = 1;
    }
  }

  if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == 0)
  {
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == 0)
    {
      value = 2;
    }
  }

  if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == 0)
  {
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2) == 0)
    {
      value = 3;
    }
  }

  if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0)
  {
    HAL_Delay(10);
    if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == 0)
    {
      value = 4;
    }
  }
  return value;
}

void keyFunc(void)
{
  uint8_t keyValue = keyRead();
  if (keyValue != button)
  {
    button = keyValue;
  }else
  {
    keyValue = 0;
  }
	if (button == 1)
	{
		usartSend();
	}
}

void lcdFunc(void)
{
  if (lcdMs < 100) return;
  lcdMs = 0;
  sprintf((char*)lcdBuffer,"Reset: %02u",romReadBuffer);
  lcdDisplayLine(lcdBuffer,Line1);
  sprintf((char*)lcdBuffer,"test: %02u",test);
  lcdDisplayLine(lcdBuffer,Line2);
  sprintf((char*)lcdBuffer,"str:%s",usartData);
  lcdDisplayLine(lcdBuffer,Line3);
}

void usartFunc(void) 
{
    // 读取UART接收的数据
    if (usartReceive(&usartBuffer) > 0) 
    {
        // 检查是否可以存储接收到的数据
        if (usartDataNum < sizeof(usartData)) 
        {
            if (usartBuffer == '\n' || usartBuffer == '\r') 
            { 
                usartData[usartDataNum] = '\0'; 
                usartDataNum = 0; // 重置计数器，准备接收下一条
                if (strcmp(usartData,"ACK") == 0)
                {
                  usartSend();
                }
                lcdClearScreen(0x0000);
                return;
            }
            usartData[usartDataNum++] = usartBuffer; // 将接收到的字节存储到 usartData 中
        }
    }
}
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
  MX_ADC2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  romWriteBuffer = 10;
  // eepromWrite((uint8_t *)&romWriteBuffer,sizeof(uint8_t));
  // eepromRead((uint8_t *)&romReadBuffer,sizeof(uint8_t));
  // if (romReadBuffer == 0)
  // {
  //   test = 1;
  //   romWriteBuffer = 1;
  //   romReadBuffer = 1;
  //   eepromWrite(&romWriteBuffer,sizeof(uint8_t));
  // }else
  // {
  //   romWriteBuffer = romReadBuffer + 1;
  //   romReadBuffer += 1;
  //   eepromWrite(&romWriteBuffer,sizeof(uint8_t));
  // }
  // I2C_Write((uint8_t*)&ucCNT,0,1);
  // HAL_Delay(3);
  // I2C_Read((uint8_t *)&test,0,1);
  // ucCNT++;
  // I2C_Write((uint8_t*)&ucCNT,0,1);
  lcdInit();
  lcdClearScreen(0x0000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    usartFunc();
    lcdFunc();
    keyFunc();
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
