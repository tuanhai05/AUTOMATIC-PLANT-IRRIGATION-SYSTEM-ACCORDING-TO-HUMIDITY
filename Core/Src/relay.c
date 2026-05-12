/* relay.c */
#include "relay.h"

static uint8_t relay_state = 0; // 0: OFF, 1: ON

void Relay_Init(void)
{
    // T?t bom khi kh?i d?ng
    Relay_Off();
}

void Relay_On(void)
{
   
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);
    relay_state = 1;
}

void Relay_Off(void)
{
    HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
    relay_state = 0;
}

uint8_t Relay_IsOn(void)
{
	
    return relay_state;
}
