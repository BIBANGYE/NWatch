#include "common.h"


void drawBattery()
{
//	uint battLevel =    battery_voltage(); 
    uint battLevel = 2300;
	const byte* battIcon;
	if(battLevel < 3100)
		battIcon = battIconEmpty;
	else if(battLevel < 3200)
		battIcon = battIconLow;
	else if(battLevel < 3300)
		battIcon = battIconHigh;
	else
		battIcon = battIconFull;

	draw_bitmap(0, FRAME_HEIGHT - 8, battIcon, 16, 8, NOINVERT, 0);
}
