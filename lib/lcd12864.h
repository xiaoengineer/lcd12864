#ifndef __LCD12864_H
#define __LCD12864_H

void Lcd_WriteCmd(unsigned char cmd); //д����
void Lcd_WriteData(unsigned char dat); //д����
void Lcd_Init(); //LCD�ĳ�ʼ��
void Lcd_Clear();//LCD����ָ��
unsigned char Lcd12864_Write16CnCHAR(unsigned char x, unsigned char y, unsigned char *cn); //��ʾ����
unsigned char Lcd12864_WriteNum8(unsigned char x, unsigned char y, unsigned char num); //��ʾ8λ�޷�������
unsigned char Lcd12864_WriteNum16(unsigned char x, unsigned char y, unsigned int num); //��ʾ16λ�޷�������
unsigned char Lcd12864_WriteString(unsigned char x, unsigned char y, unsigned char *str); //��ʾһ���ַ���
unsigned char PrintFloat(unsigned char x, unsigned char y, float num);//��ʾ���3λС��,����ض�
unsigned char PrintInteger(unsigned char x, unsigned char y, int num); //��ʾ����
unsigned char LCD_print(unsigned char row, const char *fmt, ...); //print����
#endif
