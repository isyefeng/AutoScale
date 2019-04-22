#ifndef __DISPLAY_H__
#define	__DISPLAY_H__

#include "include_type.h"
#include "global_driver.h"

void DspTimetip(void);
void DspWeight(void);
void DspTime(uchar* date,uchar* time);
void ModeInitDisplay(void);
void DspClear(void);		//Çå³ýÆÁÄ»
void DspUserMode(void);
void DspUserCheak(void);
void DspUserxInfo(uchar* temp, uchar user, char num);

#endif
