#ifndef __LCD12864_H
#define __LCD12864_H

void Lcd_WriteCmd(unsigned char cmd); //写命令
void Lcd_WriteData(unsigned char dat); //写数据
void Lcd_Init(); //LCD的初始化
void Lcd_Clear();//LCD清屏指令
unsigned char Lcd12864_Write16CnCHAR(unsigned char x, unsigned char y, unsigned char *cn); //显示汉字
unsigned char Lcd12864_WriteNum8(unsigned char x, unsigned char y, unsigned char num); //显示8位无符号整数
unsigned char Lcd12864_WriteNum16(unsigned char x, unsigned char y, unsigned int num); //显示16位无符号整数
unsigned char Lcd12864_WriteString(unsigned char x, unsigned char y, unsigned char *str); //显示一个字符串
unsigned char PrintFloat(unsigned char x, unsigned char y, float num);//显示最多3位小数,否则截断
unsigned char PrintInteger(unsigned char x, unsigned char y, int num); //显示整数
unsigned char LCD_print(unsigned char row, const char *fmt, ...); //print函数
#endif
