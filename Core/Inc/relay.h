/* relay.h */
#ifndef __RELAY_H__
#define __RELAY_H__

#include "stm32f1xx_hal.h"

/* PC13 di?u khi?n relay.
 * Gi? s?: m?c 0 -> b?t relay (bom ON), m?c 1 -> t?t.
 * N?u m?ch b?n ngu?c thì ch? c?n d?i trong Relay_On/Relay_Off.
 */

#define RELAY_GPIO_Port   GPIOC
#define RELAY_Pin         GPIO_PIN_13

void Relay_Init(void);
void Relay_On(void);
void Relay_Off(void);
uint8_t Relay_IsOn(void);

#endif /* __RELAY_H__ */
