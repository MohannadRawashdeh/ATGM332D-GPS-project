/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static void button_on_PA05_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PA05, button_on_PA05_pressed);
}

/**
 * Example of using GPS_UART to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_GPS_UART[12] = "Hello World!";

static void tx_cb_GPS_UART(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void GPS_UART_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&GPS_UART, USART_ASYNC_TXC_CB, tx_cb_GPS_UART);
	/*usart_async_register_callback(&GPS_UART, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&GPS_UART, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&GPS_UART, &io);
	usart_async_enable(&GPS_UART);

	io_write(io, example_GPS_UART, 12);
}

/**
 * Example of using TARGET_IO to write "Hello World" using the IO abstraction.
 */
void TARGET_IO_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&TARGET_IO, &io);
	usart_sync_enable(&TARGET_IO);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

/**
 * Example of using SPI_OLED to write "Hello World" using the IO abstraction.
 */
static uint8_t example_SPI_OLED[12] = "Hello World!";

void SPI_OLED_example(void)
{
	struct io_descriptor *io;
	spi_m_sync_get_io_descriptor(&SPI_OLED, &io);

	spi_m_sync_enable(&SPI_OLED);
	io_write(io, example_SPI_OLED, 12);
}

void delay_example(void)
{
	delay_ms(5000);
}

static struct timer_task TIMER_0_task1, TIMER_0_task2;

/**
 * Example of using TIMER_0.
 */
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_0_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_0_example(void)
{
	TIMER_0_task1.interval = 100;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 200;
	TIMER_0_task2.cb       = TIMER_0_task2_cb;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
}
