/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMD21 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define Button GPIO(GPIO_PORTA, 5)
#define LED2_Grn GPIO(GPIO_PORTA, 6)
#define LED3_Oge GPIO(GPIO_PORTA, 7)
#define PA16 GPIO(GPIO_PORTA, 16)
#define PA17 GPIO(GPIO_PORTA, 17)
#define DC_PIN GPIO(GPIO_PORTA, 20)
#define RES_PIN GPIO(GPIO_PORTA, 21)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define USB_DM GPIO(GPIO_PORTA, 24)
#define USB_DP GPIO(GPIO_PORTA, 25)
#define PB16 GPIO(GPIO_PORTB, 16)
#define CS_PIN GPIO(GPIO_PORTB, 17)
#define PB22 GPIO(GPIO_PORTB, 22)
#define PB23 GPIO(GPIO_PORTB, 23)

#endif // ATMEL_START_PINS_H_INCLUDED
