/* lcd1602.c */
#include "lcd1602.h"

/* Ghi 1 byte ra các chân D0..D7 (mapping r?i r?c) */
static void LCD_WriteBus(uint8_t value)
{
    HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin,
                      (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin,
                      (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin,
                      (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin,
                      (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin,
                      (value & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin,
                      (value & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin,
                      (value & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin,
                      (value & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


/* T?o xung EN */
static void LCD_PulseEnable(void)
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

/* rs=0: l?nh, rs=1: d? li?u */
static void LCD_Write(uint8_t value, uint8_t rs)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin,
                      rs ? GPIO_PIN_SET : GPIO_PIN_RESET);
    LCD_WriteBus(value);
    LCD_PulseEnable();
}

void LCD_WriteCommand(uint8_t cmd)
{
    LCD_Write(cmd, 0);
    HAL_Delay(2);
}

void LCD_WriteData(uint8_t data)
{
    LCD_Write(data, 1);
}

void LCD_Clear(void)
{
    LCD_WriteCommand(0x01);
    HAL_Delay(2);
}

void LCD_Home(void)
{
    LCD_WriteCommand(0x02);
    HAL_Delay(2);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr = (row == 0) ? (0x00 + col) : (0x40 + col);
    LCD_WriteCommand(0x80 | addr);
}

void LCD_WriteChar(char c)
{
    LCD_WriteData((uint8_t)c);
}

void LCD_WriteString(const char *str)
{
    while (*str)
    {
        LCD_WriteChar(*str++);
    }
}

void LCD_Init(void)
{
    HAL_Delay(40);          // d?i LCD kh?i d?ng
 // Thêm chu?i reset 3 l?n 0x30
    LCD_WriteCommand(0x30);
    HAL_Delay(5);
    LCD_WriteCommand(0x30);
    HAL_Delay(1);
    LCD_WriteCommand(0x30);
    HAL_Delay(1);

    LCD_WriteCommand(0x38); // 8-bit, 2 dòng, 5x8
    HAL_Delay(50);
    LCD_WriteCommand(0x0C); // display ON, cursor OFF
    HAL_Delay(50);
    LCD_WriteCommand(0x06); // entry mode
    HAL_Delay(50);

    LCD_Clear();
}
