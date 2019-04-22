#include "Display.h"

void DspWeight(void)
{
	DisplayString(2,0,"����:     g");
	lcd_pos(2,3);
	lcd_wdat(Weight_Shiwu/1000 + '0');
	lcd_wdat(Weight_Shiwu%1000/100 + '0');
	lcd_wdat(Weight_Shiwu%100/10 + '0');
	lcd_wdat(Weight_Shiwu%10 + '0');
}

void DspClear(void)
{
	lcd_wcmd(0x01);      //���LCD����ʾ����
	delay(10);
}

void DspTimetip(void)
{
	DisplayString(0,0,"Date: 20");
	DisplayString(1,0,"Time:");
	DisplayString(3,0,"K1����    K2��ѯ");
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
	DisplayString(0,0,"��ӭʹ�ã�");
	DisplayString(1,0,"���ܵ��ӳ�");
}

void DspUserMode(void)
{
	DspClear();
	DisplayString(0,0,"��ѡ��Ҫ�������");
	DisplayString(1,0,"�����û�");
	DisplayString(2,0,"K1����һҳ");
	DisplayString(3,0,"K2��ȷ��");
}

void DspUserCheak(void)
{
	DspClear();
	DisplayString(0,0,"��ѡ��Ҫ�鿴����");
	DisplayString(1,0,"�����û�");
	DisplayString(2,0,"K1����һҳ");
	DisplayString(3,0,"K2��ȷ��");
}

void DspUserxInfo(uchar* temp, uchar user, char num)
{
	long wigth_buf = 0;
	DspClear();
	DisplayString(0,0,"�û�      ��  ��");
	DisplayString(2,0,"���γ��أ�");
	DisplayString(3,0,"�ϴγ��أ�");
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
	DisplayString(1,7,"ʱ");
	
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
