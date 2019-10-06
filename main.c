#include <STC15F2K60S2.H>
#include "lcd12864.h"

void main(void)
{
	Lcd_Init();
	Lcd_Clear();
	while(1)
	{
			int i = 100;
			//LCD_print("abc:%d",123);
		LCD_print(0,"   Hello World!");
		LCD_print(1,"***************");
		LCD_print(2,"123",i,1.0);
		LCD_print(3,"abc:%dcm%f",i,1.0);
	}
}
