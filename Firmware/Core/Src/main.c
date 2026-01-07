/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "ssd1306.h"
#include "fonts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Globale Variablen (außerhalb der Funktion deklarieren)
uint32_t adc1_pulse_count = 0;
uint32_t adc2_pulse_count = 0;
uint32_t coincidence_count = 0;
uint32_t start_time = 0;

uint8_t adc1_in_pulse = 0;
uint8_t adc2_in_pulse = 0;
uint32_t adc1_sum = 0;
uint32_t adc1_sum_count = 0;
uint32_t adc2_sum = 0;
uint32_t adc2_sum_count = 0;

uint32_t adc1_pulse_start = 0;
uint32_t adc2_pulse_start = 0;

#define PULSE_THRESHOLD_HIGH 300
#define PULSE_THRESHOLD_LOW 100
#define COINCIDENCE_WINDOW_US 100000  // 50µs Zeitfenster für Koinzidenz
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
I2C_HandleTypeDef hi2c1;

long counter = 0;
uint8_t written = 0;
volatile long adc_callback_counter = 0;
#define ADCBUFSIZE 1000
uint32_t adcbuf[ADCBUFSIZE];
uint8_t adc_ready = 0;

char log_filename[32];
uint32_t file_counter = 0;
volatile uint32_t tim2_overflow = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_I2C2_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void MX_I2C1_Init(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Hilfsfunktionen für CS-Pin (PA15)
#define SD_CS_LOW()   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define SD_CS_HIGH()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)
// Timer für Mikrosekunden (z.B. TIM2 mit 1MHz = 1µs pro Tick)
// In CubeMX: Timer mit Prescaler so einstellen, dass 1 Tick = 1µs
#define GET_US_TICK() __HAL_TIM_GET_COUNTER(&htim2)

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
  MX_SPI1_Init();
  MX_FATFS_Init();
  MX_USB_DEVICE_Init();
  MX_I2C2_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  MX_I2C1_Init();
  // Kurze Wartezeit, bis USB erkannt wird
  HAL_Delay(1000);

  // String, den wir senden wollen
      serial_print("USB OK!");

      // Init lcd using one of the stm32HAL i2c typedefs
      ssd1306_Init(&hi2c1);

      //some variables for FatFs
          FATFS FatFs; 	//Fatfs handle
          FIL fil; 		//File handle
          FRESULT fres; //Result after operations

          //Open the file system
          fres = f_mount(&FatFs, "", 1); //1=mount now
          if (fres != FR_OK) {
          	serial_print("sd error!\n");
          } else {
          	serial_print("sd ok!\n");
          }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //uint16_t adc1_max = 0;
  //uint16_t adc2_max = 0;

  //HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start_IT(&htim2);
  //start_time = HAL_GetTick();

          // definitions for upper and lower threshold
            // (detector is triggered if ADC value hits upper threshold)
            // (all values are buffered until ADC value hits lower threshold)
            // (then the mean value of these values is calculated)
            uint16_t upper_thresh = 500;
            uint16_t lower_thresh = 200;

            // define allowed coincidence time difference
            uint16_t coincidence_window = 100;

            uint8_t triggered = 0;
            uint8_t triggered2 = 0;

            uint32_t sumval = 0;
            uint32_t conv_cnt = 0;
            uint16_t res_val = 0;

            uint32_t sumval2 = 0;
            uint32_t conv_cnt2 = 0;
            uint16_t res_val2 = 0;

            uint32_t c1_evt_cnt = 0;
            uint32_t c2_evt_cnt = 0;
            uint32_t coinc_evt_cnt = 0;

            uint32_t times1[100];
            uint8_t counter1 = 0;
            float rate1 = 0;

            uint32_t times2[100];
            uint8_t counter2 = 0;
            float rate2 = 0;

            uint32_t coinc_times[20];
            uint8_t coinc_count = 0;
            float coinc_rate = 0;

            uint32_t last_count1 = 0;
            uint32_t last_count2 = 0;

            // show status on display
            ssd1306_Fill(Black);

            ssd1306_UpdateScreen(&hi2c1);

            float vref = 3.3f; // reference voltage

            uint32_t microsecs = micros32();
            uint32_t microsecs2 = micros32();

            ssd1306_SetCursor(0, 0);
            ssd1306_WriteString("Rate 1: ...", Font_7x10, White);
            ssd1306_SetCursor(0, 15);
            ssd1306_WriteString("Rate 2: ...", Font_7x10, White);
            ssd1306_SetCursor(0, 30);
            ssd1306_WriteString("Myonen: 0", Font_7x10, White);
            ssd1306_SetCursor(0, 45);
            ssd1306_WriteString("Myonen-Rate: ...", Font_7x10, White);

            ssd1306_UpdateScreen(&hi2c1);
        	HAL_ADC_Start(&hadc1);
        	HAL_ADC_Start(&hadc2);

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// ADC readout
	char buf[40];
	HAL_ADC_PollForConversion(&hadc1, 1);
	HAL_ADC_PollForConversion(&hadc2, 1);
	uint16_t val = HAL_ADC_GetValue(&hadc1);
	uint16_t val2 = HAL_ADC_GetValue(&hadc2);

	// process data from 1st ADC
		      if (val >= upper_thresh) {
		    	  triggered = 1;
		    	  sumval = sumval + val;
		    	  conv_cnt++;
		    	  microsecs = micros32();
		      } else if ((val < lower_thresh) && (triggered==1)) {
		    	  res_val = (uint16_t)round(sumval/conv_cnt);

		    	  float voltage = (res_val / 4095.0f) * vref;
		     	  //sprintf(buf, "%lu | %.3f V   ", microsecs, voltage);
		     	  //ssd1306_SetCursor(0, 0);
		     	  //ssd1306_WriteString(buf, Font_7x10, White);
		     	  //ssd1306_UpdateScreen(&hi2c1);

		          times1[counter1] = microsecs;
		          counter1++;

		          last_count1 = microsecs;
		          if (counter1 == 100) {
		        	  uint32_t timediff = times1[99]-times1[0];
		        	  float timediff_in_min = timediff / 60000000.0f;
		        	  rate1 = 100/timediff_in_min;

		        	  char buf[20];
		        	  sprintf(buf, "Rate 1: %.1f", rate1);
		        	  ssd1306_SetCursor(0, 0);
		        	  ssd1306_WriteString(buf, Font_7x10, White);
		        	  ssd1306_UpdateScreen(&hi2c1);
		        	  counter1 = 0;
		          }

		          char usbbuf[20];
		          uint8_t buflen = sprintf(usbbuf, "0,%lu,%.3f\n", microsecs, voltage);
		          logToSD("log.txt", usbbuf);
		          //logToSD(usbbuf, buflen);
		          CDC_Transmit_FS((uint8_t*)usbbuf, buflen);

		          c1_evt_cnt++;

		          if ((c1_evt_cnt % 100) == 0) {
		        	  //findNewFilename();
		          }

		    	  sumval = 0;
		    	  conv_cnt = 0;
		     	  triggered = 0;

			      //sprintf(buf, "%lu", c1_evt_cnt);
			      //ssd1306_SetCursor(0, 30);
			      //ssd1306_WriteString(buf, Font_7x10, White);
			      //ssd1306_UpdateScreen(&hi2c1);
		      }

		      // process data from 2nd ADC
		      if (val2 >= upper_thresh) {
		    	  triggered2 = 1;
		      	  sumval2 = sumval2 + val2;
		      	  conv_cnt2++;
		    	  microsecs2 = micros32();
		      } else if ((val2 < lower_thresh) && (triggered2 == 1)) {
		    	  res_val2 = (uint16_t)round(sumval2/conv_cnt2);

		    	  float voltage2 = (res_val2 / 4095.0f) * vref;
		     	  //sprintf(buf, "%lu | %.3f V   ", microsecs2, voltage2);
		     	  //ssd1306_SetCursor(0, 15);
		     	  //ssd1306_WriteString(buf, Font_7x10, White);
		     	  //ssd1306_UpdateScreen(&hi2c1);

		          times2[counter2] = microsecs2;
		          counter2++;

		          last_count2 = microsecs2;

		          if (counter2 == 100) {
		        	  uint32_t timediff = times2[99]-times2[0];
		          	  float timediff_in_min = timediff / 60000000.0f;
		          	  rate2 = 100/timediff_in_min;

		          	  char buf[20];
		          	  sprintf(buf, "Rate 2: %.1f", rate2);
		          	  ssd1306_SetCursor(0, 15);
		          	  ssd1306_WriteString(buf, Font_7x10, White);
		          	  ssd1306_UpdateScreen(&hi2c1);

		          	  counter2 = 0;
		          }

		          //char usbbuf[20];
		          //uint8_t buflen = sprintf(usbbuf, "1,%lu,%.3f\n", microsecs2, voltage2);
		          //logToSD(usbbuf, buflen);
		          //CDC_Transmit_FS((uint8_t*)usbbuf, buflen);
		          char usbbuf[20];
		          uint8_t buflen = sprintf(usbbuf, "1,%lu,%.3f\n", microsecs2, voltage2);
		          //logToSD(usbbuf, buflen);
		          logToSD("log.txt", usbbuf);
		          CDC_Transmit_FS((uint8_t*)usbbuf, buflen);

		          c2_evt_cnt++;
		    	  sumval2 = 0;
		    	  conv_cnt2 = 0;
		     	  triggered2 = 0;

		     	  //sprintf(buf, "%lu", c2_evt_cnt);
		     	  //ssd1306_SetCursor(45, 30);
		     	  //ssd1306_WriteString(buf, Font_7x10, White);
		     	  //ssd1306_UpdateScreen(&hi2c1);
		      }

		      if ((last_count1 > 0) && (last_count2 > 0)) {
		    	  if (abs(last_count1-last_count2) < coincidence_window) {
		    		  coinc_evt_cnt++;

		    		  coinc_times[coinc_count] = last_count1;
					  coinc_count++;

		    		  sprintf(buf, "Myonen: %lu", coinc_evt_cnt);
		    		  ssd1306_SetCursor(0, 30);
		    		  ssd1306_WriteString(buf, Font_7x10, White);
		    		  ssd1306_UpdateScreen(&hi2c1);

		    		  last_count1 = 0;
		    		  last_count2 = 0;
		    	  }

		    	  if (coinc_count == 10) {
		    		  uint32_t timediff = coinc_times[9]-coinc_times[0];
		    		  float timediff_in_min = timediff / 60000000.0f;
		    		  coinc_rate = 10/timediff_in_min;

		    		  char buf[20];
		    		  sprintf(buf, "Myonen-Rate: %.1f", coinc_rate);
		    		  ssd1306_SetCursor(0, 45);
		    		  ssd1306_WriteString(buf, Font_7x10, White);
		    		  ssd1306_UpdateScreen(&hi2c1);

		    		  coinc_count = 0;
		    	  }
		      }
	//ADC_PulseDetection();
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void MX_I2C1_Init(void)
{
  // GPIO Pins für I2C1 Remap aktivieren
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_I2C1_CLK_ENABLE();
  __HAL_RCC_AFIO_CLK_ENABLE();

  // I2C1 Remap aktivieren
  __HAL_AFIO_REMAP_I2C1_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // PB8 (SCL) und PB9 (SDA) konfigurieren
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // I2C Parameter
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 50000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

void logToSD(const char* filename, const char* str) {
    FIL fil;
    FRESULT fres;
    char msg[128];

    // Modus setzen
    BYTE mode = FA_WRITE;
    //if (append) {
        mode |= FA_OPEN_ALWAYS;
    //} else {
    //    mode |= FA_CREATE_ALWAYS;
    //}

    // Datei öffnen
    fres = f_open(&fil, filename, mode);
    //snprintf(msg, sizeof(msg), "f_open(\"%s\", mode=0x%02X): %d\n", filename, mode, fres);
    //serial_print(msg);

    // Wenn Append, an das Ende springen
    //if (append) {
        fres = f_lseek(&fil, f_size(&fil));
        //snprintf(msg, sizeof(msg), "f_lseek to end: %d\n", fres);
        //serial_print(msg);

        if (fres != FR_OK) {
            f_close(&fil);
        }
    //}

    // Schreiben
    UINT bytesWrote = 0;
    size_t len = strlen(str);
    fres = f_write(&fil, str, len, &bytesWrote);

    //snprintf(msg, sizeof(msg), "f_write: %d, wrote %u of %u bytes\n", fres, bytesWrote, (unsigned int)len);
    //serial_print(msg);

    // Datei schließen
    FRESULT closeRes = f_close(&fil);
    //snprintf(msg, sizeof(msg), "f_close: %d\n", closeRes);
    //serial_print(msg);
}

void serial_print(const char* str) {
    //if (hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED) {
        CDC_Transmit_FS((uint8_t*)str, strlen(str));
    //}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        tim2_overflow++;
    }
}

uint32_t micros32(void)
{
    uint32_t ovf1, ovf2;
    uint16_t cnt;

    do {
        ovf1 = tim2_overflow;
        cnt  = __HAL_TIM_GET_COUNTER(&htim2);
        ovf2 = tim2_overflow;
    } while (ovf1 != ovf2);   // Overflow während des Lesens?

    return (ovf1 << 16) | cnt;
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
#ifdef USE_FULL_ASSERT
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
