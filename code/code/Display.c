#include "Display.h"

void DspWeight(void)
{
	DisplayString(2,0,"重量:     g");
	lcd_pos(2,3);
	lcd_wdat(Weight_Shiwu/1000 + '0');
	lcd_wdat(Weight_Shiwu%1000/100 + '0');
	lcd_wdat(Weight_Shiwu%100/10 + '0');
	lcd_wdat(Weight_Shiwu%10 + '0');
}

void DspClear(void)
{
	lcd_wcmd(0x01);      //清除LCD的显示内容
	delay(10);
}

void DspTimetip(void)
{
	DisplayString(0,0,"Date: 20");
	DisplayString(1,0,"Time:");
	DisplayString(3,0,"K1储存    K2查询");
}

void DspTime(uchar* date,uchar* time)
{
	lcd_pos(0,4);
	lcd_wdat(date[0]/10+'0');
	lcd_wdat(date[0]%10+'0');
	lcd_wdat('/');
	lcd_wdat(date[1]/10+'0');
	lcd_wdat(date[1]%10+'0');
	lcd_wdat('/');
	lcd_wdat(date[2]/10+'0');
	lcd_wdat(date[2]%10+'0');
	
	lcd_pos(1,3);
	lcd_wdat(time[0]/10+'0');
	lcd_wdat(time[0]%10+'0');
	lcd_wdat(':');
	lcd_wdat(time[1]/10+'0');
	lcd_wdat(time[1]%10+'0');
	lcd_wdat(':');
	lcd_wdat(time[2]/10+'0');
	lcd_wdat(time[2]%10+'0');
}

void ModeInitDisplay(void)
{
	DisplayString(0,0,"欢迎使用！");
	DisplayString(1,0,"智能电子秤");
}

void DspUserMode(void)
{
	DspClear();
	DisplayString(0,0,"请选择要储存的用");
	DisplayString(1,0,"户：用户");
	DisplayString(2,0,"K1：下一页");
	DisplayString(3,0,"K2：确认");
}

void DspUserCheak(void)
{
	DspClear();
	DisplayString(0,0,"请选择要查看的用");
	DisplayString(1,0,"户：用户");
	DisplayString(2,0,"K1：下一页");
	DisplayString(3,0,"K2：确认");
}

void DspUserxInfo(uchar* temp, uchar user, char num)
{
	long wigth_buf = 0;
	DspClear();
	DisplayString(0,0,"用户      上  次");
	DisplayString(2,0,"本次称重：");
	DisplayString(3,0,"上次称重：");
	DisplayString(1,0,"20");
	lcd_pos(0,2);
	lcd_wdat(user+'0');
	lcd_pos(0,6);
	lcd_wdat(num+1+'0');
	
	lcd_pos(1,0);
	lcd_wdat('2');
	lcd_wdat('0');
	lcd_wdat(temp[1]/10+'0');
	lcd_wdat(temp[1]%10+'0');
	lcd_wdat('/');
	lcd_wdat(temp[2]/10+'0');
	lcd_wdat(temp[2]%10+'0');
	lcd_wdat('/');
	lcd_wdat(temp[3]/10+'0');
	lcd_wdat(temp[3]%10+'0');
	lcd_pos(1,6);
	lcd_wdat(temp[4]/10+'0');
	lcd_wdat(temp[4]%10+'0');
	DisplayString(1,7,"时");
	
	lcd_pos(2,5);
	lcd_wdat(Weight_Shiwu/1000 + '0');
	lcd_wdat(Weight_Shiwu%1000/100 + '0');
	lcd_wdat(Weight_Shiwu%100/10 + '0');
	lcd_wdat(Weight_Shiwu%10 + '0');
	lcd_wdat('g');
	wigth_buf |= temp[5];
	wigth_buf = (wigth_buf<<8)|temp[6];
	lcd_pos(3,5);
	lcd_wdat(wigth_buf/1000 + '0');
	lcd_wdat(wigth_buf%1000/100 + '0');
	lcd_wdat(wigth_buf%100/10 + '0');
	lcd_wdat(wigth_buf%10 + '0');
	lcd_wdat('g');
}
