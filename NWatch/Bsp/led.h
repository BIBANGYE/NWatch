#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "typedefs.h"


typedef enum
{
	LED_RED,
	LED_GREEN
} led_t;


#define LED_FLASH_FAST	50
#define LED_BRIGHTNESS_MAX	255

//LED端口定义
#define LED0 PBout(12)// PB5
#define LED1 PBout(13)// PE5	



void led_init(void);//初始化	
void led_flash(led_t, byte, byte);
BOOL led_flashing(void);
void led_update(void);
void led_stop(void);

 




#endif



