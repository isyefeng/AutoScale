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

void DspTimetip(void)
{
	DisplayString(0,0,"Date: 20");
	DisplayString(1,0,"Time:");
}

void DspTime(uchar* date,uchar* time)
{
	lcd_pos(0,4);
	lcd_wdat(date[0]/10+'0');
	lcd_wdat(date[0]%10+'0');
	lcd_wdat(':');
	lcd_wdat(date[1]/10+'0');
	lcd_wdat(date[1]%10+'0');
	lcd_wdat(':');
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
