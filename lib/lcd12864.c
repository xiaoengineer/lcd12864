#include <STC15F2K60S2.H>
#include <intrins.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcd12864.h"

/*******端口定义*********/
#define DATA_PORT 	P0
sbit LCD12864_CS   = P3^2;
sbit LCD12864_RSET = P3^3;
sbit LCD12864_RS   = P2^6;
sbit LCD12864_RW   = P2^7;
sbit LCD12864_RD   = P2^5;


/**************/
//函数名称：Lcd_WriteCmd(unsigned char cmd)
//函数参数：unsigned char cmd
//函数说明：这个是写命令的函数
/*************/
void Lcd_WriteCmd(unsigned char cmd)
{
	LCD12864_CS = 0; //片选低电平使能
	LCD12864_RD = 1; //读失能
	LCD12864_RS = 0; //选择命令
	LCD12864_RW = 0; //选择写模式
	_nop_();
	_nop_();
	
	DATA_PORT = cmd; //放置命令
	_nop_();
	_nop_();	
	
	LCD12864_RW = 1;
}

/**************/
//函数名称：Lcd_WriteCmd(unsigned char cmd)
//函数参数：unsigned char cmd
//函数说明：这个是写数据的函数
/*************/
void Lcd_WriteData(unsigned char dat)
{
	LCD12864_CS = 0; //片选低电平使能
	LCD12864_RD = 1; //读失能
	LCD12864_RS = 1; //选择数据
	LCD12864_RW = 0; //选择写模式
	_nop_();
	_nop_();
	
	DATA_PORT = dat; //放置命令
	_nop_();
	_nop_();	
	
	LCD12864_RW = 1;
}

/**************/
//函数名称：Lcd_WriteCmd(unsigned char cmd)
//函数参数：unsigned char cmd
//函数说明：这个是写数据的函数
/*************/
void Lcd_Init()
{
	unsigned char i;
	
	LCD12864_RSET = 0; //复位一下
	for(i = 0; i < 100; i ++);
	LCD12864_CS = 0; //
	LCD12864_RSET = 1; //以上相当于是硬件初始化
	
	Lcd_WriteCmd(0XE2); //发送复位命令
	for(i = 0; i < 100; i ++); //延时一会儿
	Lcd_WriteCmd(0XA0); //0xA0(左右) 方向选择正常方向(0xA1)
	Lcd_WriteCmd(0XC8); //0xc8普通（上下）模式 0xc0正常方向
	Lcd_WriteCmd(0XA6); //0xA6 为设置字体为黑色， 背景为白色 0xA7反过来
	Lcd_WriteCmd(0XA4); //0xA4 像素正常显示， 0xA5 像素全开
	Lcd_WriteCmd(0XA2); //0xA3 偏压为 1/7,0xA2 偏压为 1/9
	
	Lcd_WriteCmd(0XF8); //这个是个双字节的命令,0xF800 选择增压为4X;
	Lcd_WriteCmd(0X01); //0xf801选择增压5X
	
	Lcd_WriteCmd(0X81); //这个是个双字节命令，高字节为 0X81
	Lcd_WriteCmd(0X23); //低字节为0x00~0xff,用来调节对比度
	
	Lcd_WriteCmd(0X25); //选择调节电阻率
	Lcd_WriteCmd(0X2F); //电源设置
	for(i = 0; i <100; i ++); //延时一会，反应一会儿
	Lcd_WriteCmd(0X40); //设置显示开始位置
	Lcd_WriteCmd(0Xaf); //开启显示
	for(i = 0; i <100; i ++);
}

void Lcd_Clear()
{
	unsigned char i, j;
	
	for(i = 0; i <8; i ++)
	{
		//--Y 轴有 64 个， 一个坐标 8 位， 也就是有 8 个坐标--//
		//所以一般我们使用的也就是从 0xB0 到 0xB7,就够了--//
		
		Lcd_WriteCmd(0xB0 + i);
		
		//--当你的段初始化为 0xA1 时， X 坐标从 0x10,0x04 到 0x18,0x04,一共 128 位--//
		//--当你的段初始化为 0xA0 时， X 坐标从 0x10,0x00 到 0x18,0x00,一共 128 位--//
		//--在写入数据之后 X 坐标的坐标是会自动加 1 的， 我们初始化使用 0xA0 所以--//
		//--我们的 X 坐标从 0x10,0x00 开始---//
		Lcd_WriteCmd(0x10);
		Lcd_WriteCmd(0x04);
		for(j = 0; j < 128; j ++)
		{
				Lcd_WriteData(0x00);
		}
	}
}


#ifdef  CHAR_CODE

#include"font.h"
unsigned char Lcd12864_Write16CnCHAR(unsigned char x, unsigned char y, unsigned char *cn)
{
	unsigned char j, x1, x2, wordNum;
	
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	Lcd_WriteCmd(y);
	while ( *cn != '\0')	 //在C语言中字符串结束以‘\0’结尾
	{	
	
		//--设置Y坐标--//
		Lcd_WriteCmd(y);

		x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
		x2 = x & 0x0F;          //去低四位
		//--设置X坐标--//
		Lcd_WriteCmd(0x10 + x1);   //高4位
		Lcd_WriteCmd(0x04 + x2);	//低4位

		for (wordNum=0; wordNum<50; wordNum++)
		{
		    //--查询要写的字在字库中的位置--//
			if ((CN16CHAR[wordNum].Index[0] == *cn)
			     &&(CN16CHAR[wordNum].Index[1] == *(cn+1)))
			{
				for (j=0; j<32; j++) //写一个字
				{		
					if (j == 16)	 //由于16X16用到两个Y坐标，当大于等于16时，切换坐标
					{
						//--设置Y坐标--//
			   			Lcd_WriteCmd(y + 1);
			
						//--设置X坐标--//
						Lcd_WriteCmd(0x10 + x1);   //高4位
						Lcd_WriteCmd(0x04 + x2);	//低4位
					}
					Lcd_WriteData(CN16CHAR[wordNum].Msk[j]);
				}
				x += 16;
			}//if查到字结束		
		} //for查字结束	
		cn += 2;
	}	//while结束
	return 1;
}

static unsigned char Lcd12864_Writechar(unsigned char x, unsigned char y, unsigned char cn)
{
	unsigned char x1, x2;
	char j;
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}	
	cn = cn - ' ';
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	Lcd_WriteCmd(y);
	//--设置X坐标--//
	x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
	x2 = x & 0x0F;   
	Lcd_WriteCmd(0x10 + x1);   //高4位
	Lcd_WriteCmd(0x04 + x2);	//低4位
	for (j=0; j<16; j++) //写一个字
	{		
		if (j == 8)	 //
		{
			//--设置Y坐标--//
			Lcd_WriteCmd(y + 1);
			
			//--设置X坐标--//
			Lcd_WriteCmd(0x10 + x1);   //高4位
			Lcd_WriteCmd(0x04 + x2);	//低4位
		}
			Lcd_WriteData(ASCII[cn * 16 + j]);
	}
	return 1;	
}

unsigned char Lcd12864_WriteString(unsigned char x, unsigned char y, unsigned char *str)
{
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}	

	while(*str != '\0')
  {       
		Lcd12864_Writechar(x,y,*str);
		x+=8;
		str++;
  }
	return 1;
}

unsigned char Lcd12864_WriteNum8(unsigned char x, unsigned char y, unsigned char num)
{
	unsigned char x1, x2, wordNum, cnt = 0;
	char j;
	unsigned char tmp_num[3];
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{		
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	
	if(num > 255)
	{
		Lcd12864_WriteString(x,y,"OverFlow");
		return 0;
	}
	
	for(j = 2; j >= 0; j --)
	{
		tmp_num[j] = num%10;
		num = num/10;
	}
	
	for(j = 0; j < 3; j ++)
	{
			if(tmp_num[j] == 0)
			{
				cnt = j+1;
			}
			else
				break;
	}
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	Lcd_WriteCmd(y);
	for(wordNum = cnt; wordNum < 3; wordNum ++)
	{
	
		//--设置Y坐标--//
		Lcd_WriteCmd(y);
		x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
		x2 = x & 0x0F;          //去低四位
		//--设置X坐标--//
		Lcd_WriteCmd(0x10 + x1);   //高4位
		Lcd_WriteCmd(0x04 + x2);	//低4位

		for (j=0; j<16; j++) //写一个字
		{		
			if (j == 8)	 //
			{
					//--设置Y坐标--//
				Lcd_WriteCmd(y + 1);
			
					//--设置X坐标--//
				Lcd_WriteCmd(0x10 + x1);   //高4位
				Lcd_WriteCmd(0x04 + x2);	//低4位
			}
				Lcd_WriteData(number[16*tmp_num[wordNum] +j]);
		}
		x += 8;		
	}
	return 1;
}

unsigned char Lcd12864_WriteNum16(unsigned char x, unsigned char y, unsigned int num)
{
	unsigned char x1, x2, wordNum,cnt= 0;
	char j;
	unsigned char tmp_num[5];
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	
	if(num > 65536)
	{
		Lcd12864_WriteString(x,y,"OverFlow");
		return 0;
	}
	
	for(j = 4; j >= 0; j --)
	{
		tmp_num[j] = num%10;
		num = num/10;
	}
	for(j = 0; j < 5; j ++)
	{
			if(tmp_num[j] == 0)
			{
				cnt = j+1;
			}
			else
				break;
	}
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	Lcd_WriteCmd(y);
	for(wordNum = cnt; wordNum < 5; wordNum ++)
	{
	
		//--设置Y坐标--//
		Lcd_WriteCmd(y);
		x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
		x2 = x & 0x0F;          //去低四位
		//--设置X坐标--//
		Lcd_WriteCmd(0x10 + x1);   //高4位
		Lcd_WriteCmd(0x04 + x2);	//低4位

		for (j=0; j<16; j++) //写一个字
		{		
			if (j == 8)	 //
			{
					//--设置Y坐标--//
				Lcd_WriteCmd(y + 1);
			
					//--设置X坐标--//
				Lcd_WriteCmd(0x10 + x1);   //高4位
				Lcd_WriteCmd(0x04 + x2);	//低4位
			}
				Lcd_WriteData(number[16*tmp_num[wordNum] +j]);
		}
		x += 8;		
	}
	return 1;
}

unsigned char PrintFloat(unsigned char x, unsigned char y, float num)
{
	char temp_char[12];
	unsigned char i, res = 0;
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	sprintf(temp_char, "%.3f", num);
	Lcd12864_WriteString(x,y,temp_char);
	for(i = 0; i < 11; i ++)
	{
		if(temp_char[i] == '\0')
		{
			break;
		}
		else
			res += 8;
	}
	return res;
}

unsigned char PrintInteger(unsigned char x, unsigned char y, int num)
{
	char temp_char[12];
	unsigned char i, res = 0;
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	sprintf(temp_char, "%d", num);
	Lcd12864_WriteString(x,y,temp_char);
	for(i = 0; i < 11; i ++)
	{
		if(temp_char[i] == '\0')
		{
			break;
		}
		else
			res += 8;
	}
	return res;
}


/*==========下面是LCD_print函数的编写===============*/

//typedef char * va_list;
//#define _INTSIZEOF(n)     ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
//	
//#define va_start(ap,v)   (ap = (va_list)&v + _INTSIZEOF(v))
//#define va_arg(ap, t)    (*(t *)(ap = ap + __INTSIZEOF(t), ap - _INTSIZEOF(t)))
//#define va_end(ap)           (ap = (va_list)0)

static unsigned char my_vprintf(unsigned char row, const char *fmt, va_list ap)
{
	char lead = ' ';
	int maxwidth = 0;
	unsigned char print_x = 0;
	unsigned char print_y = row *2; //坐标值
	for(; *fmt != '\0'; fmt ++)
	{
		if(*fmt != '%')
		{
			Lcd12864_Writechar(print_x, print_y, *fmt);
			print_x += 8;
			continue;
		}
		fmt ++;
		if(*fmt == '0')
		{
			lead = '0';
			fmt ++;
		}
		
		while(*fmt >= '0' && *fmt <= '9')
		{
			maxwidth *= 10;
			maxwidth += (*fmt - '0');
			fmt ++;
		}
		
		switch(*fmt)
		{
			case 'd': print_x +=PrintInteger(print_x, print_y, va_arg(ap,int)); break;
			case 'f': print_x +=PrintFloat(print_x, print_y,va_arg(ap, float));break;
			default:
				Lcd12864_Writechar(print_x, print_y, *fmt); break;
		}
			
	}
	return 0;
}

unsigned char LCD_print(unsigned char row, const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	my_vprintf(row,fmt,ap);
	va_end(ap);
	return 0;
}
#endif
