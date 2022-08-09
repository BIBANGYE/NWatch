#include "include.h"
#include "common.h"
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
    rtc_init();
    appconfig_init();
//    alarm_init();


    millis_init(); // 定时器
    time_init(); // 设置初始化时钟时间

    display_set(watchface_normal);
    display_load();

    OLED_ColorTurn(appConfig.invert); // 反显
}

//static stopwatch_state_t state;
static ulong timer;
static millis_t lastMS;

void stopwatch_update(void)
{
//	if(state == STATE_TIMING)
    {
        millis_t now = millis();
        timer += now - lastMS;
        lastMS = now;

        if(timer > 359999999) // 99 hours, 59 mins, 59 secs, 999 ms
            timer = 359999999;
    }
}

static void c_loop(void)
{

    time_update();

    #if COMPILE_STOPWATCH
    stopwatch_update();
    #endif

    //    if(pwrmgr_userActive())
    {
//        alarm_update();
        display_update();
    }
}











