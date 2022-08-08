#include "common.h"
#include "include.h"


#define LENTH(Buffer) sizeof(Buffer)	 		  	//数组长度	

#define EEPROM_CHECK_NUM 0x1234

#define BKPCheck_SAVE_ADDR   BKP_1_START_ADDR 	//设置BKP第一个地址保持第一次读写的标志位

appconfig_s appConfig;     //appconfig_s的长度为8


u16  BKPbuf[8];

uint32_t BKPDataRR[] = {BKP_2_START_ADDR, BKP_3_START_ADDR, BKP_4_START_ADDR, BKP_5_START_ADDR,
                        BKP_6_START_ADDR, BKP_7_START_ADDR, BKP_8_START_ADDR, BKP_9_START_ADDR
                       };
uint32_t BKPDataPAGE[] = {BKP_2_PAGE, BKP_3_PAGE, BKP_4_PAGE, BKP_5_PAGE,
                          BKP_6_PAGE, BKP_7_PAGE, BKP_8_PAGE, BKP_9_PAGE
                         };


void appconfig_init()
{
    u16 BKPCheck  ;


    BKPCheck = *((__IO uint32_t *)BKPCheck_SAVE_ADDR);

    appConfig = *(appconfig_s *) malloc(sizeof(appconfig_s));
    memset(&appConfig, 0x00, LENTH(appconfig_s));

//    if(BKPCheck == EEPROM_CHECK_NUM)
//    {
//        int index = 0;
//        appConfig.sleepTimeout = *((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//        appConfig.invert       = *((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//        #if COMPILE_ANIMATIONS
//        appConfig.animations  =  *((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//        #endif
//        appConfig.display180  =  *((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//        appConfig.CTRL_LEDs   =  *((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//        appConfig.showFPS     =  *((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//        appConfig.timeMode    = (timemode_t) * ((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//        appConfig.volumes     =  *((__IO uint32_t *)BKPDataRR[index]);
//        index++;
//    }

//    else
    {
//        BKPCheck = EEPROM_CHECK_NUM;
//        _in_flash_erase(BKP_1_PAGE, 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPCheck_SAVE_ADDR, (uint16_t)BKPCheck);

//        memset(&BKPbuf, 0x0000, LENTH(BKPbuf));

//        int index = 0;
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;
//        #if COMPILE_ANIMATIONS
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;
//        #endif
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;
//        _in_flash_erase(BKPDataPAGE[index], 1);
//        _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
//        index++;

//        appconfig_reset();
    }

//    if(appConfig.sleepTimeout > 12)
//        appConfig.sleepTimeout = 0;
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
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
    index++;
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
    index++;
    #if COMPILE_ANIMATIONS
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
    index++;
    #endif
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
    index++;
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
    index++;
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
    index++;
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
    index++;
    _in_flash_erase(BKPDataPAGE[index], 1);
    _in_flash_program(FLASH_TYPEPROGRAM_WORD, BKPDataRR[index], (uint16_t)BKPbuf[index]);
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
    appConfig.showFPS = true;
    appConfig.timeMode = TIMEMODE_24HR;
    appConfig.volumes = 255;

    appconfig_save();

//    alarm_reset();
}
