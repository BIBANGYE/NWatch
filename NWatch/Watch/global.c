/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#include "common.h"

u8 UDADDR = 0;  //用作判断usb连接

void global_init()
{
    detect_usb();

}

void global_update()
{

}

// usb连接检测
void detect_usb(void)
{
// 没有实现       
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_6; // USB_DP
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6) == 1)
    {
        UDADDR = 1; // usb端口正在使用
        return;
    }
    // usb端口没有使用
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    UDADDR = 0;
    return;
}

// Dividing by 10 and mod 10 is done a lot in the code.
// Putting them in their own functions makes the code smaller, but slightly slower.
byte div10(byte val)
{
    return val / 10;
}

byte mod10(byte val)
{
    return val % 10;
}
