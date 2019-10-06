#include <STC15F2K60S2.H>
#include <intrins.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "lcd12864.h"

/*******�˿ڶ���*********/
#define DATA_PORT 	P0
sbit LCD12864_CS   = P3^2;
sbit LCD12864_RSET = P3^3;
sbit LCD12864_RS   = P2^6;
sbit LCD12864_RW   = P2^7;
sbit LCD12864_RD   = P2^5;


/**************/
//�������ƣ�Lcd_WriteCmd(unsigned char cmd)
//����������unsigned char cmd
//����˵���������д����ĺ���
/*************/
void Lcd_WriteCmd(unsigned char cmd)
{
	LCD12864_CS = 0; //Ƭѡ�͵�ƽʹ��
	LCD12864_RD = 1; //��ʧ��
	LCD12864_RS = 0; //ѡ������
	LCD12864_RW = 0; //ѡ��дģʽ
	_nop_();
	_nop_();
	
	DATA_PORT = cmd; //��������
	_nop_();
	_nop_();	
	
	LCD12864_RW = 1;
}

/**************/
//�������ƣ�Lcd_WriteCmd(unsigned char cmd)
//����������unsigned char cmd
//����˵���������д���ݵĺ���
/*************/
void Lcd_WriteData(unsigned char dat)
{
	LCD12864_CS = 0; //Ƭѡ�͵�ƽʹ��
	LCD12864_RD = 1; //��ʧ��
	LCD12864_RS = 1; //ѡ������
	LCD12864_RW = 0; //ѡ��дģʽ
	_nop_();
	_nop_();
	
	DATA_PORT = dat; //��������
	_nop_();
	_nop_();	
	
	LCD12864_RW = 1;
}

/**************/
//�������ƣ�Lcd_WriteCmd(unsigned char cmd)
//����������unsigned char cmd
//����˵���������д���ݵĺ���
/*************/
void Lcd_Init()
{
	unsigned char i;
	
	LCD12864_RSET = 0; //��λһ��
	for(i = 0; i < 100; i ++);
	LCD12864_CS = 0; //
	LCD12864_RSET = 1; //�����൱����Ӳ����ʼ��
	
	Lcd_WriteCmd(0XE2); //���͸�λ����
	for(i = 0; i < 100; i ++); //��ʱһ���
	Lcd_WriteCmd(0XA0); //0xA0(����) ����ѡ����������(0xA1)
	Lcd_WriteCmd(0XC8); //0xc8��ͨ�����£�ģʽ 0xc0��������
	Lcd_WriteCmd(0XA6); //0xA6 Ϊ��������Ϊ��ɫ�� ����Ϊ��ɫ 0xA7������
	Lcd_WriteCmd(0XA4); //0xA4 ����������ʾ�� 0xA5 ����ȫ��
	Lcd_WriteCmd(0XA2); //0xA3 ƫѹΪ 1/7,0xA2 ƫѹΪ 1/9
	
	Lcd_WriteCmd(0XF8); //����Ǹ�˫�ֽڵ�����,0xF800 ѡ����ѹΪ4X;
	Lcd_WriteCmd(0X01); //0xf801ѡ����ѹ5X
	
	Lcd_WriteCmd(0X81); //����Ǹ�˫�ֽ�������ֽ�Ϊ 0X81
	Lcd_WriteCmd(0X23); //���ֽ�Ϊ0x00~0xff,�������ڶԱȶ�
	
	Lcd_WriteCmd(0X25); //ѡ����ڵ�����
	Lcd_WriteCmd(0X2F); //��Դ����
	for(i = 0; i <100; i ++); //��ʱһ�ᣬ��Ӧһ���
	Lcd_WriteCmd(0X40); //������ʾ��ʼλ��
	Lcd_WriteCmd(0Xaf); //������ʾ
	for(i = 0; i <100; i ++);
}

void Lcd_Clear()
{
	unsigned char i, j;
	
	for(i = 0; i <8; i ++)
	{
		//--Y ���� 64 ���� һ������ 8 λ�� Ҳ������ 8 ������--//
		//����һ������ʹ�õ�Ҳ���Ǵ� 0xB0 �� 0xB7,�͹���--//
		
		Lcd_WriteCmd(0xB0 + i);
		
		//--����Ķγ�ʼ��Ϊ 0xA1 ʱ�� X ����� 0x10,0x04 �� 0x18,0x04,һ�� 128 λ--//
		//--����Ķγ�ʼ��Ϊ 0xA0 ʱ�� X ����� 0x10,0x00 �� 0x18,0x00,һ�� 128 λ--//
		//--��д������֮�� X ����������ǻ��Զ��� 1 �ģ� ���ǳ�ʼ��ʹ�� 0xA0 ����--//
		//--���ǵ� X ����� 0x10,0x00 ��ʼ---//
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
	
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{
		return 0;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
	if(x > 128)
	{
		return 0;
	}
	y += 0xB0;	   //��ȡY�����ֵ
	//--����Y����--//
	Lcd_WriteCmd(y);
	while ( *cn != '\0')	 //��C�������ַ��������ԡ�\0����β
	{	
	
		//--����Y����--//
		Lcd_WriteCmd(y);

		x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
		x2 = x & 0x0F;          //ȥ����λ
		//--����X����--//
		Lcd_WriteCmd(0x10 + x1);   //��4λ
		Lcd_WriteCmd(0x04 + x2);	//��4λ

		for (wordNum=0; wordNum<50; wordNum++)
		{
		    //--��ѯҪд�������ֿ��е�λ��--//
			if ((CN16CHAR[wordNum].Index[0] == *cn)
			     &&(CN16CHAR[wordNum].Index[1] == *(cn+1)))
			{
				for (j=0; j<32; j++) //дһ����
				{		
					if (j == 16)	 //����16X16�õ�����Y���꣬�����ڵ���16ʱ���л�����
					{
						//--����Y����--//
			   			Lcd_WriteCmd(y + 1);
			
						//--����X����--//
						Lcd_WriteCmd(0x10 + x1);   //��4λ
						Lcd_WriteCmd(0x04 + x2);	//��4λ
					}
					Lcd_WriteData(CN16CHAR[wordNum].Msk[j]);
				}
				x += 16;
			}//if�鵽�ֽ���		
		} //for���ֽ���	
		cn += 2;
	}	//while����
	return 1;
}

static unsigned char Lcd12864_Writechar(unsigned char x, unsigned char y, unsigned char cn)
{
	unsigned char x1, x2;
	char j;
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{
		return 0;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
	if(x > 128)
	{
		return 0;
	}	
	cn = cn - ' ';
	y += 0xB0;	   //��ȡY�����ֵ
	//--����Y����--//
	Lcd_WriteCmd(y);
	//--����X����--//
	x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
	x2 = x & 0x0F;   
	Lcd_WriteCmd(0x10 + x1);   //��4λ
	Lcd_WriteCmd(0x04 + x2);	//��4λ
	for (j=0; j<16; j++) //дһ����
	{		
		if (j == 8)	 //
		{
			//--����Y����--//
			Lcd_WriteCmd(y + 1);
			
			//--����X����--//
			Lcd_WriteCmd(0x10 + x1);   //��4λ
			Lcd_WriteCmd(0x04 + x2);	//��4λ
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

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
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
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{		
		return 0;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
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
	y += 0xB0;	   //��ȡY�����ֵ
	//--����Y����--//
	Lcd_WriteCmd(y);
	for(wordNum = cnt; wordNum < 3; wordNum ++)
	{
	
		//--����Y����--//
		Lcd_WriteCmd(y);
		x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
		x2 = x & 0x0F;          //ȥ����λ
		//--����X����--//
		Lcd_WriteCmd(0x10 + x1);   //��4λ
		Lcd_WriteCmd(0x04 + x2);	//��4λ

		for (j=0; j<16; j++) //дһ����
		{		
			if (j == 8)	 //
			{
					//--����Y����--//
				Lcd_WriteCmd(y + 1);
			
					//--����X����--//
				Lcd_WriteCmd(0x10 + x1);   //��4λ
				Lcd_WriteCmd(0x04 + x2);	//��4λ
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
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{
		return 0;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
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
	y += 0xB0;	   //��ȡY�����ֵ
	//--����Y����--//
	Lcd_WriteCmd(y);
	for(wordNum = cnt; wordNum < 5; wordNum ++)
	{
	
		//--����Y����--//
		Lcd_WriteCmd(y);
		x1 = (x >> 4) & 0x0F;   //����X����Ҫ��������ָߵ�4λ������������ȡ����4λ
		x2 = x & 0x0F;          //ȥ����λ
		//--����X����--//
		Lcd_WriteCmd(0x10 + x1);   //��4λ
		Lcd_WriteCmd(0x04 + x2);	//��4λ

		for (j=0; j<16; j++) //дһ����
		{		
			if (j == 8)	 //
			{
					//--����Y����--//
				Lcd_WriteCmd(y + 1);
			
					//--����X����--//
				Lcd_WriteCmd(0x10 + x1);   //��4λ
				Lcd_WriteCmd(0x04 + x2);	//��4λ
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
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{
		return 0;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
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
	//--Y������ֻ�ܴ�0��7��������ֱ�ӷ���--//
	if(y > 7)
	{
		return 0;
	}

	//--X������ֻ�ܴ�0��128��������ֱ�ӷ���--//
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


/*==========������LCD_print�����ı�д===============*/

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
	unsigned char print_y = row *2; //����ֵ
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
