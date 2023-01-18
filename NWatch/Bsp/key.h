#ifndef __KEY_H
#define __KEY_H	 

#include "include.h"

#define KEY0  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_10)   //读取按键 上
#define KEY1  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_2)  //读取按键  下
#define KEY2  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)   //读取按键 确认
#define KEY3  HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_4)   //读取按键3(WK_UP) 复位

void KEY_Init(void);//IO初始化


#endif
