#ifndef __LCD12864_H__
#define	__LCD12864_H__

#include<reg52.h>
#include<intrins.h>
#include "include_type.h"

#define LCD_data  P0             //数据口
sbit LCD_RS  =  P2^0;            //寄存器选择输入 
sbit LCD_RW  =  P2^1;            //液晶读/写控制
sbit LCD_EN  =  P2^2;            //液晶使能控制
sbit LCD_PSB =  P2^3;            //串/并方式控制

bit lcd_busy();
void lcd_wcmd(uchar cmd);
void lcd_wdat(uchar dat);
void lcd_pos(uchar X,uchar Y);
void lcd_init();
void DisplayString(uchar x, uchar y, uchar* String);

#endif
