/* main.c */

#include "main.h"

#include "lcd1602.h"
#include "sensor.h"
#include "relay.h"
#include <stdio.h>

/* Global ADC handle */
ADC_HandleTypeDef hadc1;

/* Hàm c?u hình */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);

/* Bi?n dùng cho chuong trình */
char line1[17];
char line2[17];
float do_am;
uint8_t pump_on;

int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_ADC1_Init();

  LCD_Init();
  Relay_Init();
	 __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_NOJTAG();  // T?t JTAG, gi?i phóng PA15, PB3, PB4 cho LCD
/* Màn hình chào lúc kh?i d?ng */
  LCD_Clear();
  LCD_SetCursor(0, 0);          // dòng 1, c?t 0
  LCD_WriteString("BTL-ESD");

  LCD_SetCursor(1, 0);          // dòng 2, c?t 0
  LCD_WriteString("HOC KI 251");

  HAL_Delay(2000);              // hi?n 2 giây
  /* ADC1 ? ch? d? Continuous, Start 1 l?n */
  HAL_ADC_Start(&hadc1);

  while (1)
  {
    /* 1. Ð?c d? ?m (PA0 -> ADC1_IN0) */
    do_am = Sensor_ReadPercent(&hadc1);

    /* 2. Ði?u khi?n bom theo ngu?ng 40% */
    if (do_am < 50.0f)
    {
      Relay_On();   // PC13 = 1 -> bom ON (active-high)
    }
    else
    {
      Relay_Off();  // PC13 = 0 -> bom OFF
    }
    pump_on = Relay_IsOn();

    /* 3. Hi?n th? lên LCD */
    snprintf(line1, sizeof(line1), "DO KHO DAT:%3.0f%%", do_am);
    snprintf(line2, sizeof(line2), "MAY BOM:%s", pump_on ? "ON " : "OFF");

   
    LCD_SetCursor(0, 0);
    LCD_WriteString(line1);

    LCD_SetCursor(1, 0);
    LCD_WriteString(line2);

    HAL_Delay(500);
  }
}

/* ================= CLOCK CONFIG (HSI 8 MHz) ================= */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /* Clock cho ADC */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection    = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* ================= ADC1: PA0 - Continuous mode ================= */
static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  __HAL_RCC_ADC1_CLK_ENABLE();

  hadc1.Instance                   = ADC1;
  hadc1.Init.ScanConvMode          = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode    = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion       = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel      = ADC_CHANNEL_0;              // PA0
  sConfig.Rank         = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;  // m?u d? dài
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

  /* Calibration cho F1 – r?t quan tr?ng */
  if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* ================= GPIO: LCD + ADC + RELAY ================= */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* PA0: analog cho ADC */
  GPIO_InitStruct.Pin  = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PC13: relay output */
  GPIO_InitStruct.Pin   = GPIO_PIN_13;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PA1, PA2: LCD RS, EN */
  GPIO_InitStruct.Pin   = GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_12 | GPIO_PIN_15;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PB0..PB7: LCD D0..D7 */
  GPIO_InitStruct.Pin   = GPIO_PIN_3 | GPIO_PIN_4 |
                          GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Giá tr? kh?i t?o */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2|GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB,
         GPIO_PIN_3 | GPIO_PIN_4 |
        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8,
        GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // bom OFF lúc d?u
}

/* ================= Error Handler ================= */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
