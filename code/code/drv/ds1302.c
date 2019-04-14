#include "ds1302.h"
sbit Led = P1^0;
/********************************************************************/ 
/*单字节写入一字节数据*/
void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	for (i=0;i<8;i++) 
	{ 
		if (dat & 0x01) 	// 等价于if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*电平置高*/
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*电平置低*/
		}		 
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*单字节读出一字节数据*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //等价于if(SDA_R==1)    #define SDA_R SDA /*电平读取*/	
		{
			dat |= 0x80;
		}
		else 
		{
			dat &= 0x7F;
		}
		SCK_SET;
		SCK_CLR;
	}
	return dat;
}

/********************************************************************/ 
/*向DS1302 单字节写入一字节数据*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 
	unsigned char temp = 0;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;			/*启动DS1302总线,RST=1电平置高 */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是写操作,写之前将最低位置零*/	
	temp = ((dat/10)<<4)|(dat%10);
	Write_Ds1302_Byte(temp);	 /*写入数据：dat*/
	SDA_CLR;
	RST_CLR;				 /*停止DS1302总线*/
}

/********************************************************************/ 
/*从DS1302单字节读出一字节数据*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	unsigned char date;
	RST_CLR;			/*RST脚置低，实现DS1302的初始化*/
	SCK_CLR;			/*SCK脚置低，实现DS1302的初始化*/

	RST_SET;	/*启动DS1302总线,RST=1电平置高 */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*写入目标地址：addr,保证是读操作,写之前将最低位置高*/
	temp=Read_Ds1302_Byte(); /*从DS1302中读出一个字节的数据*/		
	date = ((temp>>4)*10) + (temp&0x0f);
	SDA_CLR;
	RST_CLR;	/*停止DS1302总线*/
	return date;
}

void Ds1302_Date_set(unsigned char* Date)
{
	Ds1302_Single_Byte_Write(0x8e, 0x00);
	Ds1302_Single_Byte_Write(ds1302_date_addr, Date[2]);
	Ds1302_Single_Byte_Write(ds1302_month_addr, Date[1]);
	Ds1302_Single_Byte_Write(ds1302_year_addr, Date[0]);
	Ds1302_Single_Byte_Write(0x8e, 0x80);
}

void Ds1302_time_set(unsigned char* time)
{
	Ds1302_Single_Byte_Write(0x8e, 0x00);
	Ds1302_Single_Byte_Write(ds1302_sec_addr, time[2]);
	Ds1302_Single_Byte_Write(ds1302_min_addr, time[1]);
	Ds1302_Single_Byte_Write(ds1302_hr_addr, time[0]);
	Ds1302_Single_Byte_Write(0x8e, 0x80);
}

void Ds1302_time_read(unsigned char* date)
{
//	Ds1302_Single_Byte_Write(0x8e, 0x00);
	date[0] = Ds1302_Single_Byte_Read(ds1302_hr_addr);
	date[1] = Ds1302_Single_Byte_Read(ds1302_min_addr);
	date[2] = Ds1302_Single_Byte_Read(ds1302_sec_addr);
//	Ds1302_Single_Byte_Write(0x8e, 0x80);
}

void Ds1302_Date_read(unsigned char* date)
{
	date[0] = Ds1302_Single_Byte_Read(ds1302_year_addr);
	date[1] = Ds1302_Single_Byte_Read(ds1302_month_addr);
	date[2] = Ds1302_Single_Byte_Read(ds1302_date_addr);
}
