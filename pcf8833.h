/* 
 *
 *
 */

#ifndef PCF8833_H
#define PCF8833_H

#include <stdint.h>

typedef enum 
{
	LCD_FONT_BASIC,
	LCD_FONT_SGA
} LCD_FONT;

#define PCF8833_EXIT_SLEEP		(0x11)

#define PCF8833_BOOSTER_ON		(0x03)
#define PCF8833_DISPLAY_ON		(0x29)

#define PCF8833_SET_CONTRAST		(0x25)
//Default contrast level is 0x30, works well on most screens
#define PCF8833_CONTRAST_LEVEL		(0x30) 

#define PCF8833_COLUMM_ADDRESS_SET	(0x2A)
#define PCF8833_ROW_ADDRESS_SET		(0x2B)
#define PCF8833_RAM_WRITE		(0x2C)

#define PCF8833_PIXEL_FORMAT		(0x3A)
#define PCF8833_PIXEL_FORMAT_16BIT	(0b101)

#define COLOR_BLACK	(0x0000)
#define COLOR_WHITE	(0xFFFF)



uint8_t lcd_reverse_byte(uint8_t data);

void lcd_wait();
void lcd_assert_reset();
void lcd_assert_ce();
void lcd_clear_reset();
void lcd_clear_ce();

void lcd_init();

void lcd_command(uint8_t data);
void lcd_data(uint8_t data);

void lcd_color(uint16_t color);
void lcd_mono_byte(uint8_t data);

void lcd_set_window( uint8_t x, uint8_t y, uint8_t width, uint8_t height);
void lcd_fill(uint16_t color, uint8_t x, uint8_t y, uint8_t width, uint8_t height);

void lcd_character_font(const char font[26][7], uint8_t x, uint8_t y, char c);
void lcd_character(uint8_t x, uint8_t y, char c);
void lcd_character_sga(uint8_t x, uint8_t y, char c);
void lcd_string(uint8_t font, uint8_t x, uint8_t y, char * s);



#endif
