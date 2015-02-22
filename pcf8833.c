/* Driver for Philips PCF8833 based LCDs such as the Nokia 6100
 * 132px by 132px resolution, 12 bit color
 *
 */

#include <stdint.h>
#include "stm32f10x.h"

#include "system.h"
#include "pcf8833.h"
#include "font8x7_A-Z.h"
#include "font8x7_sga.h"

/*
 * The PCF8833 requires a 9 bit SPI interface sent in MSB first order 
 *
 * The USART is the only peripheral that does 9 bit synchronous serial
 * on the STM32F103. It sends data LSB first.
 *
 * In order to avoid recomputing the commands from the datasheet the
 * function lcd_reverse_byte() was written
 */
uint8_t lcd_reverse_byte(uint8_t data)
{
   data = (data & 0xF0) >> 4 | (data & 0x0F) << 4;
   data = (data & 0xCC) >> 2 | (data & 0x33) << 2;
   data = (data & 0xAA) >> 1 | (data & 0x55) << 1;
   
   return data;
}

/*
 * A handful of microseconds worth of delay
 */
void lcd_wait()
{
	//1024 times
	for (uint32_t i=0;!(i&0x400);i++)
		asm("nop");
}

inline void lcd_assert_reset()
{
	//Clear reset pin (active low)
	GPIOA->BRR=1;
}

inline void lcd_assert_ce()
{
	//Clear chip enable pin (active low)
	GPIOA->BRR=2;
}

inline void lcd_clear_reset()
{
	//Set reset pin (active low)
	GPIOA->BSRR=1;
}

inline void lcd_clear_ce()
{
	//Set chip enable pin (active low)
	GPIOA->BSRR=2;
}

void lcd_init()
{
	lcd_clear_reset();
	lcd_wait();
	lcd_assert_reset();
	lcd_wait();
	lcd_clear_reset();
	lcd_wait();
	
	
	lcd_clear_ce();
	lcd_wait();
	lcd_assert_ce();
	lcd_wait();
	
	// Exit Sleep
	lcd_command(PCF8833_EXIT_SLEEP);
	
	// Set Contrast
	lcd_command(PCF8833_SET_CONTRAST);
	lcd_data(PCF8833_CONTRAST_LEVEL);
		
	//COLOR INTERFACE PIXEL FORMAT, 16 bit
	lcd_command(PCF8833_PIXEL_FORMAT);
	lcd_data(PCF8833_PIXEL_FORMAT_16BIT);
	
	lcd_fill(COLOR_BLACK, 0, 0, 132, 132);
	
	//booster voltage on
	lcd_command(PCF8833_BOOSTER_ON);
	//display on
	lcd_command(PCF8833_DISPLAY_ON);
}

void lcd_command(uint8_t data)
{
	//9 bit commands start with the first bit zero
	// d dddd ddd0
	usart1_send(lcd_reverse_byte(data) << 1);
}

void lcd_data(uint8_t data)
{
	//9 bit data packets start with the first bit one
	// d dddd ddd1
	usart1_send((lcd_reverse_byte(data) << 1) | 1);
}

/*
	16 bit Input color
	5:6:5 format
	Bits 4:0   - Blue
	Bits 10:5  - Green
	Bits 15:11 - Red
*/
void lcd_color(uint16_t color)
{
	//7:4 Red
	//3:0 Green
	lcd_data( (color>>8) & 0xFF);
	
	//7:5 Green
	//4:0 Blue
	lcd_data( color & 0xFF );
}

void lcd_mono_byte(uint8_t data)
{
	for(uint8_t i=0; i<8; i++)
	{

		if (data&1)
			lcd_color(COLOR_WHITE);
		else
			lcd_color(COLOR_BLACK);
			
		data=data>>1;
	}
}

void lcd_set_window( uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	//column address set
	lcd_command(PCF8833_COLUMM_ADDRESS_SET);
	lcd_data(x);
	lcd_data(x+width-1);
	
	//page address set
	lcd_command(PCF8833_ROW_ADDRESS_SET);
	lcd_data(y);
	lcd_data(y+height-1);
	
	//RAMWR
	lcd_command(PCF8833_RAM_WRITE);
}

void lcd_fill(uint16_t color, uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
	lcd_set_window( x, y, width, height);
	
	for(uint8_t i=0;i<width;i++)
	{	
		for(uint8_t j=0;j<height;j++)
		{
			lcd_color(color);
		}
	}
}

//input character must be an uppercase ASCII letter or space
void lcd_character_font(const char font[26][7], uint8_t x, uint8_t y, char c)
{
	lcd_set_window( x, y, 8, 7);

	for(uint8_t i=0; i<7; i++)
	{
		if (c!=' ')
			lcd_mono_byte(font[c-65][i]);
		else
			lcd_mono_byte(0);
	}
}

inline void lcd_character(uint8_t x, uint8_t y, char c)
{
	lcd_character_font(font8x7_AZ, x, y, c);
}

inline void lcd_character_sga(uint8_t x, uint8_t y, char c)
{
	lcd_character_font(font8x7_sga, x, y, c);
}

void lcd_string(uint8_t font, uint8_t x, uint8_t y, char * s)
{
	while(*s)
	{
		if (font==0)
			lcd_character_font(font8x7_AZ, x, y, *s);
		else
			lcd_character_font(font8x7_sga, x, y, *s);
		
		s++;
		
		x+=8;
		if (x>132-8)
			break;
	}
}
