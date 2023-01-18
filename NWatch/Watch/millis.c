#include "millis.h"

millis_t milliseconds;

TIM_HandleTypeDef htim6;

void millis_init(void)
{
    __HAL_RCC_TIM6_CLK_ENABLE();

    htim6.Instance = TIM6;
    htim6.Init.Prescaler = 72 - 1; // us
    htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim6.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim6.Init.Period = 1000 - 1; // 1ms
    htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    
    HAL_TIM_Base_Init(&htim6);
    HAL_NVIC_SetPriority(TIM6_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM6_IRQn);
    HAL_TIM_Base_Start_IT(&htim6);
}

// Get current milliseconds
millis_t millis_get()
{
    millis_t ms;
    ms = milliseconds;
    return ms;
}

void TIM6_IRQHandler(void)
{
    //每1毫秒进一次中断
    HAL_TIM_IRQHandler(&htim6);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM6)
    {
        ++milliseconds;
        update = true;

        if(milliseconds % 1000 == 0)
        {
            #ifndef RTC_SRC
            ++timeDate.time.secs;   //每秒钟标志位+1;
            #endif
        }
    }
}

