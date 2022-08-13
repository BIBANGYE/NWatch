#include "include.h"

static void c_setup(void);
static void c_loop(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    c_setup();

    while(1)
    {
        c_loop();

    }
}

static void c_setup(void)
{
    HAL_Init();
    SystemClock_Config(); /* 配置系统时钟为72MHz */

    #if COMPILE_UART
    DEBUG_USART_Config();		//初始化串口波特率为115200
    #endif

//    global_init();
    memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
    
    oled_init();
    led_init();
    rtc_init(); // rtc外设初始化
    appconfig_init(); // 保存配置数据
//    alarm_init();
    
    buttons_init();
    millis_init(); // 定时器初始化
    time_init();  // 设置初始时间

    display_set(watchface_normal);
    display_load();
}



static void c_loop(void)
{
    time_update();

    buttons_update();

    #if COMPILE_STOPWATCH
    stopwatch_update();
    #endif

    display_update();
}











