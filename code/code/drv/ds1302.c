#include "ds1302.h"
sbit Led = P1^0;
/********************************************************************/ 
/*���ֽ�д��һ�ֽ�����*/
void Write_Ds1302_Byte(unsigned char dat) 
{
	unsigned char i;
	SCK = 0;
	for (i=0;i<8;i++) 
	{ 
		if (dat & 0x01) 	// �ȼ���if((addr & 0x01) ==1) 
		{
			SDA_SET;		//#define SDA_SET SDA=1 /*��ƽ�ø�*/
		}
		else 
		{
			SDA_CLR;		//#define SDA_CLR SDA=0 /*��ƽ�õ�*/
		}		 
		SCK_SET;
		SCK_CLR;		
		dat = dat >> 1; 
	} 
}
/********************************************************************/ 
/*���ֽڶ���һ�ֽ�����*/
unsigned char Read_Ds1302_Byte(void) 
{
	unsigned char i, dat=0;	
	for (i=0;i<8;i++)
	{	
		dat = dat >> 1;
		if (SDA_R) 	  //�ȼ���if(SDA_R==1)    #define SDA_R SDA /*��ƽ��ȡ*/	
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
/*��DS1302 ���ֽ�д��һ�ֽ�����*/
void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat)
{ 
	unsigned char temp = 0;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;			/*����DS1302����,RST=1��ƽ�ø� */
	addr = addr & 0xFE;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤��д����,д֮ǰ�����λ����*/	
	temp = ((dat/10)<<4)|(dat%10);
	Write_Ds1302_Byte(temp);	 /*д�����ݣ�dat*/
	SDA_CLR;
	RST_CLR;				 /*ֹͣDS1302����*/
}

/********************************************************************/ 
/*��DS1302���ֽڶ���һ�ֽ�����*/
unsigned char Ds1302_Single_Byte_Read(unsigned char addr) 
{ 
	unsigned char temp;
	unsigned char date;
	RST_CLR;			/*RST���õͣ�ʵ��DS1302�ĳ�ʼ��*/
	SCK_CLR;			/*SCK���õͣ�ʵ��DS1302�ĳ�ʼ��*/

	RST_SET;	/*����DS1302����,RST=1��ƽ�ø� */	
	addr = addr | 0x01;	 
	Write_Ds1302_Byte(addr); /*д��Ŀ���ַ��addr,��֤�Ƕ�����,д֮ǰ�����λ�ø�*/
	temp=Read_Ds1302_Byte(); /*��DS1302�ж���һ���ֽڵ�����*/		
	date = ((temp>>4)*10) + (temp&0x0f);
	SDA_CLR;
	RST_CLR;	/*ֹͣDS1302����*/
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
