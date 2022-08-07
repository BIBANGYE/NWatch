#include "include.h"

void c_setup(void);
void c_loop(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{


    HAL_Init();
    SystemClock_Config(); /* 配置系统时钟为72MHz */

    #if COMPILE_UART
    DEBUG_USART_Config();		//初始化串口波特率为115200
    #endif

    time_init();

    oled_init();              //初始化oled 四线模拟spi模式
    led_init();

    while(1)
    {

    }
}









