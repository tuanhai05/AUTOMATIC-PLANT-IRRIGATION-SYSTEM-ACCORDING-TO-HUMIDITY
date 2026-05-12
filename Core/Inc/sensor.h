/* sensor.h */
#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "stm32f1xx_hal.h"
#include <stdint.h>

/* Ð?c c?m bi?n analog ? PA0 (ADC1 Channel 0)
 * Tr? v? giá tr? t? 0..100 (%)
 * B?n có th? d?i công th?c bên trong cho dúng c?m bi?n th?c t?.
 */
float Sensor_ReadPercent(ADC_HandleTypeDef *hadc);

#endif /* __SENSOR_H__ */
