#ifndef _IIC_H
#define _IIC_H

#include "reg52.h"
#include "intrins.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();}
#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P1^5;  /* 数据线 */
sbit SCL = P1^6;  /* 时钟线 */

//函数声明
void IIC_Start(void); 
void IIC_Stop(void);  
void IIC_Ack(unsigned char ackbit); 
void IIC_SendByte(unsigned char byt); 
bit IIC_WaitAck(void);  
unsigned char IIC_RecByte(void); 
unsigned char Read_2402(unsigned char addr);
void Write_2402(unsigned char addr,unsigned char dat);
unsigned char Read_8951(void);


#endif