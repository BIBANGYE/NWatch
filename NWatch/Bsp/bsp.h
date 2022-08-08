#ifndef __BSP_H_
#define __BSP_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "bsp_debug_usart.h"
#include "bsp_lcd.h"
#include "bsp_internal_flash.h"

void bsp_init(void);
void SystemClock_Config(void);

#endif

