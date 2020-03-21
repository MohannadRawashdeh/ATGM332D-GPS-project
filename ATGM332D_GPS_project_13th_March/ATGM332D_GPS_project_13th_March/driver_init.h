/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_ext_irq.h>

#include <hal_usart_async.h>

#include <hal_usart_sync.h>
#include <hal_spi_m_sync.h>

#include <hal_delay.h>
#include <hal_timer.h>
#include <hpl_tc_base.h>

#include "hal_usb_device.h"

extern struct usart_async_descriptor GPS_UART;

extern struct usart_sync_descriptor TARGET_IO;
extern struct spi_m_sync_descriptor SPI_OLED;

extern struct timer_descriptor TIMER_0;

void GPS_UART_PORT_init(void);
void GPS_UART_CLOCK_init(void);
void GPS_UART_init(void);

void TARGET_IO_PORT_init(void);
void TARGET_IO_CLOCK_init(void);
void TARGET_IO_init(void);

void SPI_OLED_PORT_init(void);
void SPI_OLED_CLOCK_init(void);
void SPI_OLED_init(void);

void delay_driver_init(void);

void TARGET_USB_CLOCK_init(void);
void TARGET_USB_init(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
