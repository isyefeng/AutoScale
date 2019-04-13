#include <reg52.h>
#include "include_type.h"
#include "global_driver.h"

void global_driver_init(void)
{
	lcd_init();
	DisplayString(0,0,"hello word!"); 
}

void main(void)
{
	global_driver_init();
	while(1)
	{
		;
	}
}