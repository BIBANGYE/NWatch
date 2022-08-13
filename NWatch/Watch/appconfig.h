/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef APPCONFIG_H_
#define APPCONFIG_H_

#include "common.h"

// Could use bitfields for the bools to save a few bytes of RAM and EEPROM, but uses an extra ~82 bytes of flash
typedef struct
{
    byte sleepTimeout;
    bool invert;
    #if COMPILE_ANIMATIONS
    bool animations;
    #endif
    bool display180;
    bool CTRL_LEDs;
    bool showFPS;
    timemode_t timeMode;
    union
    {
        byte volumes;
        struct  // get rid of these bitfields?
        {
            byte volUI: 2;
            byte volAlarm: 2;
            byte volHour: 2;
            byte brightness: 2;
        };
    };
} appconfig_s;


extern appconfig_s appConfig;

void appconfig_init(void);
void appconfig_save(void);
void appconfig_reset(void);

#endif /* APPCONFIG_H_ */
