#ifndef __KEY_H
#define __KEY_H	 

#include "include.h"

#define KEY0  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)//��ȡ���� ��
#define KEY1  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)//��ȡ����  ��
#define KEY2  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)//��ȡ���� ȷ��
#define KEY3  HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15)//��ȡ����3(WK_UP) ��λ


void KEY_Init(void);//IO��ʼ��
#endif
