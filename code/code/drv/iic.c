/*
  ����˵��: IIC������������
  �������: Keil uVision 4.10 
  Ӳ������: CT107��Ƭ���ۺ�ʵѵƽ̨(12MHz)
  ��    ��: 2011-8-9
*/

#include "iic.h"

void Delay_iic()
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 9;
	while (--i);
}

//������������
void IIC_Start(void)
{
	SDA = 1;
	SCL = 1;
	Delay_iic();
	SDA = 0;
	Delay_iic();
	SCL = 0;	
}

//����ֹͣ����
void IIC_Stop(void)
{
	SDA = 0;
	SCL = 1;
	Delay_iic();
	SDA = 1;
}

////Ӧ��λ����
//void IIC_Ack(unsigned char ackbit)
//{
//	if(ackbit) 
//	{	
//		SDA = 0;
//	}
//	else 
//	{
//		SDA = 1;
//	}
//	Delay_iic()
//	SCL = 1;
//	Delay_iic()
//	SCL = 0;
//	SDA = 1; 
//	Delay_iic()
//}

//�ȴ�Ӧ��
bit IIC_WaitAck(void)
{
	SDA = 1;
	Delay_iic();
	SCL = 1;
	Delay_iic();
	if(SDA)    
	{   
		SCL = 0;
		IIC_Stop();
		return 0;
	}
	else  
	{ 
		SCL = 0;
		return 1;
	}
}

//ͨ��I2C���߷�������
void IIC_SendByte(unsigned char byt)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{   
		if(byt&0x80) 
		{	
			SDA = 1;
		}
		else 
		{
			SDA = 0;
		}
		Delay_iic();
		SCL = 1;
		byt <<= 1;
		Delay_iic();
		SCL = 0;
	}
}

//��I2C�����Ͻ�������
unsigned char IIC_RecByte(void)
{
	unsigned char da;
	unsigned char i;
	
	for(i=0;i<8;i++)
	{   
		SCL = 1;
		Delay_iic();
		da <<= 1;
		if(SDA) 
		da |= 0x01;
		SCL = 0;
		Delay_iic();
	}
	return da;
}

void Write_2402(unsigned char addr,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();	
}

unsigned char Read_2402(unsigned char addr)
{
	unsigned char temp;
	IIC_Start();
	IIC_SendByte(0xa0);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	IIC_Stop();

	IIC_Start();
	IIC_SendByte(0xa1);
	IIC_WaitAck();
	temp=IIC_RecByte();
	IIC_Stop();	

	return temp;
}

