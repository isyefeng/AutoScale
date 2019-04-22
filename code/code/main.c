#include <reg52.h>
#include "include_type.h"
#include "global_driver.h"
#include "Display.h"

sbit LED8 = P1^7;
bit m_ds_flag = 0;
uchar m_save_num = 0;
unsigned char timebuf[3] = {10,10,10};
unsigned char Datebuf[3] = {0,0,0};

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
	lcd_wcmd(0x01);	//清除开机屏幕
	Get_Maopi();
	DspTimetip();
}

uchar get_save_addres(uchar user)
{
	return ((user-1)*3*7);
}

void DspSavenow(uchar temp)
{
	DspClear();
	DisplayString(0,0,"储存中");
	DisplayString(1,0,"第  条");
	lcd_pos(1,1);
	lcd_wdat(temp+1+'0');
	Delay_ms(1000);
	DspClear();
}

void save_user_info(uchar user)
{
	uchar IicIndex = 0;
	IicIndex = Read_2402(get_save_addres(user));
	IicIndex++;
	if(IicIndex == 3)
		IicIndex = 0;
	Write_2402(get_save_addres(user)+0, IicIndex);
	Delay_ms(20);
	Write_2402(get_save_addres(user)+1+IicIndex*7, Datebuf[0]);
	Delay_ms(20);
	Write_2402(get_save_addres(user)+2+IicIndex*7, Datebuf[1]);
	Delay_ms(20);
	Write_2402(get_save_addres(user)+3+IicIndex*7, Datebuf[2]);
	Delay_ms(20);
	Write_2402(get_save_addres(user)+4+IicIndex*7, timebuf[0]);
	Delay_ms(20);
	Write_2402(get_save_addres(user)+5+IicIndex*7, (uchar)((Weight_Shiwu&0xffff)>>8));
	Delay_ms(20);
	Write_2402(get_save_addres(user)+6+IicIndex*7, (uchar)(Weight_Shiwu&0xff));
	Delay_ms(20);
	DspSavenow(IicIndex);
}

#if 0
void test(uchar temp)
{
	uchar buf[7] = {1,1,1,1,1,1,1};
	uchar index = 0;
	uchar IicIndex = 0;
	
	IicIndex = Read_2402(get_save_addres(temp));
	Delay_ms(20);
	for(index = 0;index<7;index++)
	{
		buf[index] = Read_2402(get_save_addres(temp)+index+IicIndex*7);
		Delay_ms(20);
	}
	lcd_pos(3,0);
	for(index = 0;index<7;index++)
	{
		lcd_wdat(buf[index]/10+'0');
		lcd_wdat(buf[index]%10+'0');
	}
}
#endif

void UserSaveWight(void)
{
	uchar User = 1;
	uchar index = 0;
	DspUserMode();
	while(1)
	{
		index++;
		
		if(KEY1 == 0)
		{
			Delay_ms(20);
			if(KEY1 == 0)
			{
				while(KEY1 == 0);
				User++;
				if(User == 4)
				{
					DspClear();
					DspTimetip();
					break;
				}
			}
		}
		
		if(KEY2 == 0)
		{
			Delay_ms(20);
			if(KEY2 == 0)
			{
				while(KEY2 == 0);
				save_user_info(User);
				DspClear();
				DspTimetip();
				break;
			}
		}
		
		if(index == 200)
		{
			lcd_pos(1,4);
			lcd_wdat(User + '0');
			index = 0;
		}
	}
}

void CheakUserxInfo(uchar temp)
{
	uchar buf[7] = {1,1,1,1,1,1,1};
	uchar buf1[3] = {0,0,0};
	uchar index = 0;
	char IicIndex = 0;
	
	for(index = 0;index<7;index++)
	{
		buf[index] = Read_2402(get_save_addres(temp)+index+IicIndex*7);
		Delay_ms(20);
	}
	DspUserxInfo(buf, temp, IicIndex);
	while(1)
	{
		if(KEY3 == 0)
		{
			Delay_ms(20);
			if(KEY3 == 0)
			{
				while(KEY3 == 0);
					IicIndex++;
				if(IicIndex == 3)
					IicIndex = 0;
				for(index = 0;index<7;index++)
				{
					buf[index] = Read_2402(get_save_addres(temp)+index+IicIndex*7);
					Delay_ms(20);
				}
				DspUserxInfo(buf, temp, IicIndex);
			}
		}
		
		if(KEY4 == 0)
		{
			Delay_ms(20);
			if(KEY4 == 0)
			{
				while(KEY4 == 0);
					IicIndex--;
				if(IicIndex == -1)
					IicIndex = 2;
				for(index = 0;index<7;index++)
				{
					buf[index] = Read_2402(get_save_addres(temp)+index+IicIndex*7);
					Delay_ms(20);
				}
				DspUserxInfo(buf, temp, IicIndex);
			}
		}
		if(KEY2 == 0)
		{
			Delay_ms(20);
			if(KEY2 == 0)
			{
				while(KEY2 == 0);
				break;
			}
		}
	}
}

void cheak_wight_mode(void)
{
	uchar index = 0;
	uchar User = 1;
	DspUserCheak();
	while(1)
	{
		index++;
		if(index == 200)
		{
			lcd_pos(1,4);
			lcd_wdat(User + '0');
			index = 0;
		}
		
		if(KEY1 == 0)
		{
			Delay_ms(20);
			if(KEY1 == 0)
			{
				while(KEY1 == 0);
				User++;
				if(User == 4)
				{
					DspClear();
					DspTimetip();
					break;
				}
			}
		}
		
		if(KEY2 == 0)
		{
			Delay_ms(20);
			if(KEY2 == 0)
			{
				while(KEY2 == 0);
				CheakUserxInfo(User);
				DspClear();
				DspTimetip();
				break;
			}
		}
	}
}

void main(void)
{
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
			LED8 =~ LED8;
			m_ds_flag = 0;
		}
		if(KEY1 == 0)
		{
			Delay_ms(10);
			if(KEY1 == 0)
			{
				while(KEY1 == 0);
				UserSaveWight();
			}
		}
		
		if(KEY2 == 0)
		{
			Delay_ms(10);
			if(KEY2 == 0)
			{
				while(KEY2 == 0);
				cheak_wight_mode();
			}
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