#include "stm32f10x.h"
#include "system.h"
#include "pcf8833.h"

int main()
{
	init_clock();
	init_gpio();
	init_usart1();
	
	lcd_init();

	lcd_string(LCD_FONT_BASIC, 35,60, "HELLO THERE");
	lcd_string(LCD_FONT_SGA  , 35,70, "HELLO THERE");
	
	for (uint32_t i=0;i < 1000000;i++)
			asm("nop");
	
	int y=0;
	char c='A';
	while (1)
	{
		lcd_character(10,y, c);
		lcd_character_sga(20, y, c);
		y+=8;
		c++;
		
		if (c>'Z')
			c='A';
		
		for (uint32_t i=0;i < 100000;i++)
			asm("nop");
	}
}
