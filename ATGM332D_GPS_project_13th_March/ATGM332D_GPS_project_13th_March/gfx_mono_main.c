/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file or main.c
 * to avoid loosing it when reconfiguring.
 */
#include "atmel_start.h"
#include <hal_gpio.h>
#include "gfx_mono_main.h"
#include "atmel_start_pins.h"

/* Pins to use for Chip Select, Reset and Data/Command signals */
#ifndef MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN
#define MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN CS_PIN
#endif
#ifndef MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN
#define MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN RES_PIN
#endif
#ifndef MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN
#define MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN DC_PIN
#endif

/*! ug2832hsweg04 display structure */
static struct ug2832hsweg04 MONOCHROME_DISPLAY_0_c12832a1z_desc;
/*! Buffer used by ug2832hsweg04 display */
static uint8_t MONOCHROME_DISPLAY_0_buffer[UG2832HSWEG04_LCD_FRAMEBUFFER_SIZE];

/*! The graphics library */
struct gfx_mono MONOCHROME_GRAPHICS_0_desc;
/*! The graphics text library */
struct gfx_mono_text MONOCHROME_TEXT_0_desc;

/**
 * \brief Initialize GFX Mono library
 */
void gfx_mono_init(void)
{
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_direction(MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN, true);
	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN, true);
	gpio_set_pin_level(MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN, true);
	MONOCHROME_TEXT_0_desc.gfx->display->height=UG2832HSWEG04_LCD_HEIGHT;
	MONOCHROME_TEXT_0_desc.gfx->display->height=UG2832HSWEG04_LCD_WIDTH;
	MONOCHROME_DISPLAY_0_c12832a1z_desc.parent.height=UG2832HSWEG04_LCD_HEIGHT;
	MONOCHROME_DISPLAY_0_c12832a1z_desc.parent.width=UG2832HSWEG04_LCD_WIDTH;
	ug2832hsweg04_construct(&MONOCHROME_DISPLAY_0_c12832a1z_desc.parent,
	                        MONOCHROME_DISPLAY_0_buffer,
	                        &SPI_OLED.io,
	                        MONOCHROME_DISPLAY_CONTROLLER_0_CS_PIN,
	                        MONOCHROME_DISPLAY_CONTROLLER_0_RES_PIN,
	                        MONOCHROME_DISPLAY_CONTROLLER_0_DC_PIN);					
	gfx_mono_construct(&MONOCHROME_GRAPHICS_0_desc, &MONOCHROME_DISPLAY_0_c12832a1z_desc.parent);
	gfx_mono_text_construct(&MONOCHROME_TEXT_0_desc, &MONOCHROME_GRAPHICS_0_desc);
}
