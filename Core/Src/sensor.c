/* sensor.c */
#include "sensor.h"

float Sensor_ReadPercent(ADC_HandleTypeDef *hadc)
{
    uint32_t adc_val = 0;

 
    if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK)
    {
        adc_val = HAL_ADC_GetValue(hadc);
    }
  

    // ADC 12 bit: 0..4095
    float percent = (float)adc_val * 100.0f / 4095.0f;

    if (percent < 0.0f)   percent = 0.0f;
    if (percent > 100.0f) percent = 100.0f;

    return percent;
}
