#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "stm32f1xx_hal.h"
#include <stdint.h>

/* RS, EN */
#define LCD_RS_GPIO_Port   GPIOA
#define LCD_RS_Pin         GPIO_PIN_1

#define LCD_EN_GPIO_Port   GPIOA
#define LCD_EN_Pin         GPIO_PIN_2

/* D0..D7 – mapping r?i r?c */
#define LCD_D0_GPIO_Port   GPIOA
#define LCD_D0_Pin         GPIO_PIN_12

#define LCD_D1_GPIO_Port   GPIOA
#define LCD_D1_Pin         GPIO_PIN_15

#define LCD_D2_GPIO_Port   GPIOB
#define LCD_D2_Pin         GPIO_PIN_3   

#define LCD_D3_GPIO_Port   GPIOB
#define LCD_D3_Pin         GPIO_PIN_4

#define LCD_D4_GPIO_Port   GPIOB
#define LCD_D4_Pin         GPIO_PIN_5

#define LCD_D5_GPIO_Port   GPIOB
#define LCD_D5_Pin         GPIO_PIN_6

#define LCD_D6_GPIO_Port   GPIOB
#define LCD_D6_Pin         GPIO_PIN_7

#define LCD_D7_GPIO_Port   GPIOB
#define LCD_D7_Pin         GPIO_PIN_8

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_WriteChar(char c);
void LCD_WriteString(const char *str);
void LCD_WriteCommand(uint8_t cmd);
void LCD_WriteData(uint8_t data);

#endif /* __LCD1602_H__ */
