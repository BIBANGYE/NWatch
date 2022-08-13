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

//    global_init();
    memset(&oledBuffer, 0x00, FRAME_BUFFER_SIZE);
    
    oled_init();
    led_init();
    rtc_init(); // rtc�����ʼ��
    appconfig_init(); // ������������
//    alarm_init();
    
    buttons_init();
    millis_init(); // ��ʱ����ʼ��
    time_init();  // ���ó�ʼʱ��

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











