#ifndef __HX711DRV_H__
#define	__HX711DRV_H__

#include "include_type.h"

sbit hx711_dout=P2^7; 
sbit hx711_sck=P2^6; 

//IO…Ë÷√
sbit HX711_DOUT=P2^7; 
sbit HX711_SCK=P2^6; 

void Get_Maopi();
void Get_Weight();
void Delay_ms(unsigned int n);

#endif
