#include "led.h"
#include "millis.h"
#include "common.h"

typedef struct
{
    byte flashLen;			// How long to light up for
    millis8_t startTime;	//
} led_s;

static led_s ledRed;
static led_s ledGreen;

//LED IO初始化
void led_init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin =  GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void flash(led_s* led, byte len, byte pin)
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
                flash(&ledRed, len, 13);
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
    if((millis8_t)(millis() - led->startTime) >= led->flashLen)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // 关灯
        led->flashLen = 0;
    }
    else
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    return led->flashLen;
}

void led_update()
{
    BOOL red = _update(&ledRed, 13);
    BOOL green = _update(&ledGreen, 13);
}

// Turn off LEDs
void led_stop()
{
    led_flash(LED_GREEN, 0, 0);
    led_flash(LED_RED, 0, 0);
    led_update();
}



