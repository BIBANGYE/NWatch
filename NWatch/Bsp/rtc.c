#include "rtc.h"


RTC_HandleTypeDef Rtc_Handle;

void rtc_init(void)
{
    /*配置RTC预分频器和RTC数据寄存器 */
    /*Asynch Prediv  = 由HAL自动计算 */
    Rtc_Handle.Instance = RTC;
    Rtc_Handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;

    HAL_RTC_Init(&Rtc_Handle);
}

/**
  * @brief  RTC配置：选择RTC时钟源
  * @param  无
  * @retval 无
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /*##-1- 启用PWR时钟并启用对备份域的访问###################################*/
    /* 要更改RTC功能（LSE，LSI）的源时钟，您必须：
       - 使用__HAL_RCC_PWR_CLK_ENABLE（）启用电源时钟
       - 在使用之前使用HAL_PWR_EnableBkUpAccess（）函数启用写访问 配置RTC时钟源（复位后执行一次）。
       - 使用__HAL_RCC_BACKUPRESET_FORCE（）和重置备份域
       __HAL_RCC_BACKUPRESET_RELEASE（）。
       - 配置所需的RTc时钟源*/
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    /* 为备份寄存器启用BKP CLK */
    __HAL_RCC_BKP_CLK_ENABLE();

    /*##-2-将ISE配置为RTC时钟源 ###################################*/
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE  ;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;

    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;

    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        
    }

    /*##-3- 启用RTC外设时钟 #######################################*/
    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();
}


