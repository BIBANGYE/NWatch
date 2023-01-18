#include "include.h"

static void c_setup(void);
static void c_loop(void);

/**
  * @brief  ������
  * @param  ��
  * @retval ��
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
    SystemClock_Config(); /* ����ϵͳʱ��Ϊ72MHz */

    #if COMPILE_UART
    DEBUG_USART_Config();		//��ʼ�����ڲ�����Ϊ115200
    #endif

    memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
    oled_init();
    led_init();
    //    global_init();
    rtc_init(); // rtc�����ʼ��
    appconfig_init(); // ������������
//    alarm_init();

    buttons_init();
    millis_init(); // ��ʱ����ʼ��
    time_init();   // ����Ĭ��ʱ��

    display_set(watchface_normal);
    display_load();

    OLED_ColorTurn(appConfig.invert);
}



static void c_loop(void)
{
    time_update();

    if (pwrmgr_userActive())
    {
        buttons_update();
    }

    #if COMPILE_STOPWATCH
    stopwatch_update();
    #endif

    display_update();
}











