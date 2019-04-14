#include <reg52.h>
#include "include_type.h"
#include "global_driver.h"
#include "Display.h"

bit m_ds_flag = 0;

void Timer0Init(void)		//1000
{
	TMOD |= 0x01;		
	TL0 = (65535-1000)%256;	
	TH0 = (65535-1000)/256;	
	TF0 = 0;		
	TR0 = 1;	
	EA = 1;
	ET0 = 1;
}

void global_driver_init(void)
{
	uchar set_date[] = {19,4,14};
	uchar set_time[] = {23,59,55};
	Timer0Init();	
	lcd_init();
	ModeInitDisplay();
	Ds1302_Date_set(set_date);
	Ds1302_time_set(set_time);
	Delay_ms(3000);
	lcd_wcmd(0x01);	//Çå³ý¿ª»úÆÁÄ»
	Get_Maopi();
	DspTimetip();
}

void main(void)
{
	unsigned char timebuf[3] = {10,10,10};
	unsigned char Datebuf[3] = {0,0,0};
	global_driver_init();
	while(1)
	{
		if(m_ds_flag)
		{
			Ds1302_Date_read(Datebuf);
			Ds1302_time_read(timebuf);
			DspTime(Datebuf,timebuf);
			DspWeight();
			Get_Weight();
			m_ds_flag = 0;
		}
		
	}
}

void Tim0(void) interrupt 1
{
	static uint ds_index = 0;
	TL0 = (65535-1000)%256;	
	TH0 = (65535-1000)/256;	
	ds_index++;
	if(ds_index == 300)
	{
		ds_index = 0;
		m_ds_flag = 1;
	}
	
}