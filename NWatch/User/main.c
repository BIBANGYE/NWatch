#include "include.h"

void c_setup(void);
void c_loop(void);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{


    HAL_Init();
    SystemClock_Config(); /* ����ϵͳʱ��Ϊ72MHz */

    #if COMPILE_UART
    DEBUG_USART_Config();		//��ʼ�����ڲ�����Ϊ115200
    #endif

    time_init();

    oled_init();              //��ʼ��oled ����ģ��spiģʽ
    led_init();

    while(1)
    {

    }
}









