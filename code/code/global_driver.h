#ifndef __GLOBAL_DRIVER_H__
#define	__GLOBAL_DRIVER_H__

#include "include_type.h"
#include "lcd12864.h"
#include "ds1302.h"
#include "HX711Drv.h"
#include "iic.h"

sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;
sbit KEY4 = P3^5;

extern long Weight_Shiwu;

#endif
