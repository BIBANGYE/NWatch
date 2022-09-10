#include "appconfig.h"


#define LENTH(Buffer) sizeof(Buffer)    //计算数组长度	

#define EEPROM_CHECK_NUM 0x1234 // 数值随意设置

#define BKPCheck_SAVE_ADDR   RTC_BKP_DR1 	//设置BKP第一个地址保持第一次读写的标志位

appconfig_s appConfig;     //appconfig_s的长度为8


u16  BKPbuf[8];

uint32_t BKPDataRR[] = {RTC_BKP_DR2, RTC_BKP_DR3, RTC_BKP_DR4, RTC_BKP_DR5, RTC_BKP_DR6, RTC_BKP_DR7, RTC_BKP_DR8, RTC_BKP_DR9, RTC_BKP_DR10, RTC_BKP_DR11, RTC_BKP_DR12,
                        RTC_BKP_DR13, RTC_BKP_DR14, RTC_BKP_DR15, RTC_BKP_DR16, RTC_BKP_DR17, RTC_BKP_DR18, RTC_BKP_DR19, RTC_BKP_DR20, RTC_BKP_DR21, RTC_BKP_DR22, RTC_BKP_DR23,
                        RTC_BKP_DR24, RTC_BKP_DR25, RTC_BKP_DR26, RTC_BKP_DR27, RTC_BKP_DR28, RTC_BKP_DR29, RTC_BKP_DR30
                       };


void appconfig_init()
{
    uint16_t BKPCheck;


    BKPCheck = HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPCheck_SAVE_ADDR);

    appConfig = *(appconfig_s *) malloc(sizeof(appconfig_s));
    memset(&appConfig, 0x00, LENTH(appconfig_s));

    if(BKPCheck == EEPROM_CHECK_NUM) // 配置文件已经设置
    {
        // 读取配置文件
        int index = 0;
        appConfig.sleepTimeout = HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
        index++;
        appConfig.invert       = HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
        index++;
        #if COMPILE_ANIMATIONS
        appConfig.animations  =  HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
        index++;
        #endif
        appConfig.display180  =  HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
        index++;
        appConfig.CTRL_LEDs   =  HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
        index++;
//        appConfig.showFPS     =  HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
		appConfig.showFPS     =  0;
        index++;
        appConfig.timeMode    = (timemode_t) HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
        index++;
        appConfig.volumes     =  HAL_RTCEx_BKUPRead(&Rtc_Handle, BKPDataRR[index]);
        index++;
    }
    else
    {   // 设置配置文件
        BKPCheck = EEPROM_CHECK_NUM;
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPCheck_SAVE_ADDR, BKPCheck);

        memset(&BKPbuf, 0x0000, LENTH(BKPbuf));

        int index = 0;
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;
        #if COMPILE_ANIMATIONS
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;
        #endif
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;
        HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
        index++;

        appconfig_reset();
    }

    if(appConfig.sleepTimeout > 12)
        appConfig.sleepTimeout = 0;
}

void appconfig_save()
{

    BKPbuf[0] = (u16)appConfig.sleepTimeout;
    BKPbuf[1] = (u16)appConfig.invert ;
    #if COMPILE_ANIMATIONS
    BKPbuf[2] = (u16)appConfig.animations;
    #endif
    BKPbuf[3] = (u16)appConfig.display180;
    BKPbuf[4] = (u16)appConfig.CTRL_LEDs;
    BKPbuf[5] = (u16)appConfig.showFPS;
    BKPbuf[6] = (u16)appConfig.timeMode ;
    BKPbuf[7] = (u16)appConfig.volumes;


    int index = 0;
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
    #if COMPILE_ANIMATIONS
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
    #endif
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
    HAL_RTCEx_BKUPWrite(&Rtc_Handle, BKPDataRR[index], BKPbuf[index]);
    index++;
}

void appconfig_reset()
{
    appConfig.sleepTimeout = 1;
    appConfig.invert = false;
    #if COMPILE_ANIMATIONS
    appConfig.animations = true;
    #endif
    appConfig.display180 = true;
    appConfig.CTRL_LEDs = false;
    appConfig.showFPS = false;
    appConfig.timeMode = TIMEMODE_24HR;
    appConfig.volumes = 255;

    appconfig_save();

    alarm_reset();
}
