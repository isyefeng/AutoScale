#include "HX711Drv.h"

#define GapValue 437
sbit Buzzer = P2^3;
sbit LED = P1^0;

unsigned long HX711_Buffer = 0;
unsigned long Weight_Maopi = 0;
long Weight_Shiwu = 0;


void Delay__hx711_us(void)
{
	_nop_();
	_nop_();
}

unsigned long HX711_Read(void)	//����128
{
	unsigned long count; 
	unsigned char i; 
  HX711_DOUT=1; 
	Delay__hx711_us();
  	HX711_SCK=0; 
	count = 0;
  	while(HX711_DOUT); 
  	for(i=0;i<24;i++)
	{ 
		HX711_SCK=1; 
	  	count=count<<1; 
		HX711_SCK=0; 
	  	if(HX711_DOUT)
			count++; 
	} 
 	HX711_SCK=1; 
    count=count^0x800000;//��25�������½�����ʱ��ת������
		Delay__hx711_us();
	HX711_SCK=0;  
	return(count);
}




//****************************************************
//����
//****************************************************
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	if(Weight_Shiwu > 0)			
	{	
		Weight_Shiwu = (unsigned int)((float)Weight_Shiwu/GapValue); 	//����ʵ���ʵ������
	}
	else
	{
		Weight_Shiwu = 0;
	}
	
}

//****************************************************
//��ȡëƤ����
//****************************************************
void Get_Maopi()
{
	Weight_Maopi = HX711_Read();	
} 

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

