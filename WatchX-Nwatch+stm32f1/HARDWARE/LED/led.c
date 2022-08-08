#include "led.h"
#include "millis.h"
#include "common.h"

typedef struct {
  byte flashLen;			// How long to light up for
  millis8_t startTime;	//
} led_s;

static led_s ledRed;
static led_s ledGreen;


//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void led_init()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;				 //LED0-->PB.12 13 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

}

static void flash(led_s* led, byte len,byte pin)
{
  led->flashLen = len;
  led->startTime = millis();

}

void led_flash(led_t led, byte len, byte brightness)
{
  if (appConfig.CTRL_LEDs)        //读出来是0 但是还是有问题估计与聚合体有关
	{

    switch (led)
    {
      case LED_RED:
			flash(&ledRed, len, 12);
        break;
      case LED_GREEN:
			flash(&ledGreen, len, 13);
        break;
      default:
        break;
    }
  }
}

// Is an LED on?
BOOL led_flashing()
{
  return ledRed.flashLen || ledGreen.flashLen;
}

static BOOL _update(led_s* led,  byte pin)        //(volatile unsigned long  *)
{
  if ((millis8_t)(millis() - led->startTime) >= led->flashLen)
//	  if (led->flashLen)
  {
		PBout(pin)=0;
		led->flashLen =0;
  }
	else
		
		 PBout(pin)=1;

  return led->flashLen;
}

void led_update()
{
//  BOOL red = _update(&ledRed, &RED_CCR, _BV(RED_COM), &RED_PORT, _BV(RED_PIN));
//  BOOL green = _update(&ledGreen, &GREEN_CCR, _BV(GREEN_COM), &GREEN_PORT, _BV(GREEN_PIN));
BOOL red = _update(&ledRed,12);          //LED0 PBout(12)
BOOL green = _update(&ledGreen, 13);    //LED1 PBout(13)

	
}

// Turn off LEDs
void led_stop()
{
  led_flash(LED_GREEN, 0, 0);
  led_flash(LED_RED, 0, 0);
  led_update();
}



