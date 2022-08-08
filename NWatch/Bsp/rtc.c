#include "rtc.h"


RTC_HandleTypeDef Rtc_Handle;

void rtc_init(void)
{
    /*����RTCԤ��Ƶ����RTC���ݼĴ��� */
    /*Asynch Prediv  = ��HAL�Զ����� */
    Rtc_Handle.Instance = RTC;
    Rtc_Handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;

    HAL_RTC_Init(&Rtc_Handle);
}

/**
  * @brief  RTC���ã�ѡ��RTCʱ��Դ
  * @param  ��
  * @retval ��
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /*##-1- ����PWRʱ�Ӳ����öԱ�����ķ���###################################*/
    /* Ҫ����RTC���ܣ�LSE��LSI����Դʱ�ӣ������룺
       - ʹ��__HAL_RCC_PWR_CLK_ENABLE�������õ�Դʱ��
       - ��ʹ��֮ǰʹ��HAL_PWR_EnableBkUpAccess������������д���� ����RTCʱ��Դ����λ��ִ��һ�Σ���
       - ʹ��__HAL_RCC_BACKUPRESET_FORCE���������ñ�����
       __HAL_RCC_BACKUPRESET_RELEASE������
       - ���������RTcʱ��Դ*/
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    /* Ϊ���ݼĴ�������BKP CLK */
    __HAL_RCC_BKP_CLK_ENABLE();

    /*##-2-��ISE����ΪRTCʱ��Դ ###################################*/
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

    /*##-3- ����RTC����ʱ�� #######################################*/
    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();
}


